#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <math.h>

#include "gauss.hpp"

void LU(std::vector<std::vector<double> > A, std::vector<std::vector<double> > &L,
        std::vector<std::vector<double> > &U, int n) {
    U = A;
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++)
            L[j][i] = U[j][i] / U[i][i];
    
    for (int k = 1; k < n; k++) {
        for (int i = k - 1; i < n; i++)
            for (int j = i; j < n; j++)
                L[j][i] = U[j][i] / U[i][i];

        for (int i = k; i < n; i++)
            for (int j = k - 1; j < n; j++)
                U[i][j] = U[i][j] - L[i][k - 1] * U[k - 1][j];
    }
}

void LU_omp(std::vector<std::vector<double> > A, std::vector<std::vector<double> > &L,
    std::vector<std::vector<double> > &U, int n) {
    U = A;
    int i;
#pragma omp parallel for private(i) shared (L,U)
    for (i = 0; i < n; i++) {
        int j;
#pragma omp parallel for private(j) shared (L,U)
        for (j = i; j < n; j++)
            L[j][i] = U[j][i] / U[i][i];
    }
    for (int k = 1; k < n; k++) {
        int i;
#pragma omp parallel for private(i) shared (L,U)
        for (i = k - 1; i < n; i++) {
            int j;
#pragma omp parallel for private(j) shared (L,U)
            for (j = i; j < n; j++)
                L[j][i] = U[j][i] / U[i][i];
        }
#pragma omp parallel for private(i) shared (U,L)
        for (i = k; i < n; i++) {
            int j;
#pragma omp parallel for private(j) shared (U,L)
            for (j = k - 1; j < n; j++)
                U[i][j] = U[i][j] - L[i][k - 1] * U[k - 1][j];
        }
    }
}

std::vector<double> gauss(std::vector<std::vector<double> > L, std::vector<std::vector<double> > U, std::vector<double> b, int n) {
    std::vector<double> y(n, 0);
    std::vector<double> x(n, 0);
    y[0] = b[0] / L[0][0];
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            b[i] -= L[i][j] * y[j];
        }
        y[i] = b[i] / L[i][i];
    }
    x[n - 1] = y[n - 1] / U[n - 1][n - 1];
    for (int i = n - 2; i >= 0; --i) {
        for (int j = n - 1; j > i; --j) {
            y[i] -= U[i][j] * x[j];
        }
        x[i] = y[i] / U[i][i];
    }
    return x;
}

std::vector<double> gauss_omp(std::vector<std::vector<double> > L, std::vector<std::vector<double> > U, std::vector<double> b, int n) {
    std::vector<double> y(n, 0);
    std::vector<double> x(n, 0);
    y[0] = b[0] / L[0][0];
    int i;
    for (i = 1; i < n; ++i) {
        int j;
        double sum = 0;
#pragma omp parallel for reduction(+:sum)
        for (j = 0; j < i; ++j) {
            sum += L[i][j] * y[j];
        }
        b[i] -= sum;
        y[i] = b[i] / L[i][i];
    }
    x[n - 1] = y[n - 1] / U[n - 1][n - 1];
    for (i = n - 2; i >= 0; --i) {
        int j;
        double sum = 0;
#pragma omp parallel for reduction(+:sum)
        for (j = n - 1; j > i; --j) {
            sum += U[i][j] * x[j];
        }
        y[i] -= sum;
        x[i] = y[i] / U[i][i];
    }
    return x;
}
