#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <math.h>

#include "seidel.hpp"

const double eps = 0.001;

bool converge(std::vector<double> x, std::vector<double> x_next, int n, double eps)
{
    double norm = 0;
    for (int i = 0; i < n; i++)
        norm += (x[i] - x_next[i]) * (x[i] - x_next[i]);
    return (sqrt(norm) < eps);
}

std::vector<double> seidel(int n, std::vector<std::vector<double> > a, std::vector<double> x, std::vector<double> x_prev, std::vector<double> b) {
    do {
        for (int i = 0; i < n; i++)
            x_prev[i] = x[i];
        for (int i = 0; i < n; i++) {
            double var = 0;
            for (int j = 0; j < i; j++)
                var += (a[i][j] * x[j]);
            for (int j = i + 1; j < n; j++)
                var += (a[i][j] * x_prev[j]);
            x[i] = (b[i] - var) / a[i][i];
        }
    } while (!converge(x, x_prev, n, eps));
    return x;
}

std::vector<double> seidel_omp(int n, std::vector<std::vector<double> > a, std::vector<double> x, std::vector<double> p, std::vector<double> b) {
    do
    {
        for (int i = 0; i < n; i++)
            p[i] = x[i];
#pragma omp parallel for
        for (int i = 0; i < n; i++)
        {
            double var = 0;
#pragma omp parallel for reduction (+:var)
            for (int j = 0; j < i; j++)
                var += (a[i][j] * x[j]);
#pragma omp parallel for reduction (+:var)
            for (int j = i + 1; j < n; j++)
                var += (a[i][j] * p[j]);
            x[i] = (b[i] - var) / a[i][i];
        }
    } while (!converge(x, p, n, eps));
    return x;
}
