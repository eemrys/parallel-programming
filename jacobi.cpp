#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <math.h>

#include "jacobi.hpp"

const double eps = 0.001;

std::vector<double> jacobi(int n, std::vector<std::vector<double> > matrix_a, std::vector<double> b, std::vector<double> x) {
    std::vector<double> x_next(n, 0);
    double norm;
    do {
        for (int i = 0; i < n; i++) {
            x_next[i] = b[i];
            for (int g = 0; g < n; g++) {
                if (i != g)
                    x_next[i] -= matrix_a[i][g] * x[g];
            }
            x_next[i] /= matrix_a[i][i];
        }
        norm = fabs(x[0] - x_next[0]);
        for (int i = 0; i < n; i++) {
            if (fabs(x[i] - x_next[i]) > norm)
                norm = fabs(x[i] - x_next[i]);
            x[i] = x_next[i];
        }
    } while (norm > eps);
    return x;
}

std::vector<double> jacobi_omp(int n, std::vector<std::vector<double> > matrix_a, std::vector<double> b, std::vector<double> x) {
    std::vector<double> x_next(n, 0);
    double norm;

    for (int p = 0; p < 1; --p) {
#pragma omp parallel for
        for (int i = 0; i < n; i++) {
            x_next[i] = b[i];
            double sum = 0;
#pragma omp parallel for reduction (+:sum)
            for (int g = 0; g < n; g++) {
                if (i != g)
                    sum += matrix_a[i][g] * x[g];
            }
            x_next[i] -= sum;
            x_next[i] /= matrix_a[i][i];
        }
        norm = fabs(x[0] - x_next[0]);
#pragma omp parallel for
        for (int h = 0; h < n; h++) {
            if (fabs(x[h] - x_next[h]) > norm)
                norm = fabs(x[h] - x_next[h]);
            x[h] = x_next[h];
        }
        if (norm < eps) break;
    }
    return x;
}
