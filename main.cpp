#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <math.h>

#include "gauss.hpp"
#include "jacobi.hpp"
#include "seidel.hpp"


std::vector<std::vector<double> > create_matrix_a(const int matrix_size) {
    std::vector<std::vector<double> > new_mat(matrix_size, std::vector<double>(matrix_size));
    double after_dot = 1000.0;
    int range_max(10 * after_dot);
    int range_min(-10 * after_dot);

    for (int i = 0; i < matrix_size; ++i) {
        double sum(0.0);

        for (int j = 0; j < matrix_size; ++j) {
            double u = ((double)rand() / (RAND_MAX) * (range_max - range_min) + range_min) / after_dot;
            sum += abs(u);
            new_mat[i][j] = u;
        }
        
        if (new_mat[i][i] < 0) {
            sum *= -1;
        }
        new_mat[i][i] += sum;
    }
    return new_mat;
}

std::vector<double> create_vec_b(const int size) {
    std::vector<double> new_vector(size);
    double after_dot = 1000.0;
    int range_max(10 * after_dot);
    int range_min(-10 * after_dot);
    
    for (int i = 0; i < size; ++i) {
        new_vector[i] = ((double)rand() / (RAND_MAX) * (range_max - range_min) + range_min) / after_dot;
    }
    return new_vector;
}

void show(std::vector <double> A, int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    const int n = 4;
    const int k = 100;
    std::vector<std::vector<double> > matrix_a(n, std::vector<double>(n));

    matrix_a[0][0] = 5;
    matrix_a[0][1] = -3;
    matrix_a[0][2] = 4;
    matrix_a[0][3] = -1;
    matrix_a[1][0] = 3;
    matrix_a[1][1] = 2;
    matrix_a[1][2] = -5;
    matrix_a[1][3] = 2;
    matrix_a[2][0] = 1;
    matrix_a[2][1] = 8;
    matrix_a[2][2] = -1;
    matrix_a[2][3] = -3;
    matrix_a[3][0] = 3;
    matrix_a[3][1] = -2;
    matrix_a[3][2] = -5;
    matrix_a[3][3] = 4;
    
    //matrix_a = create_matrix_a(n);
    std::vector<double> vec_b_s(n), vec_b_p(n);
    //vec_b_s = create_vec_b(n);
    vec_b_s[0] = 7;
    vec_b_s[1] = 0;
    vec_b_s[2] = 2;
    vec_b_s[3] = 0;
    
    vec_b_p = vec_b_s;
// Gauss test
    std::vector<double> time_gauss_s(k, 0);
    std::vector<double> time_gauss_p(k, 0);
    for (int i = 0; i < k; ++i) {
        std::vector<std::vector<double> >  L_s(n, std::vector<double>()),
                                L_p(n, std::vector<double>()),
                                U_s(n, std::vector<double>()),
                                U_p(n, std::vector<double>());
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                L_s[i].push_back(0);
                L_p[i].push_back(0);
                U_s[i].push_back(0);
                U_p[i].push_back(0);
            }
        }
        int start_s = clock();
        LU(matrix_a, L_s, U_s, n);
        std::vector<double> x_s;
        x_s = gauss(L_s, U_s, vec_b_s, n);
        int end_s = clock();
        int start_p = clock();
        LU_omp(matrix_a, L_p, U_p, n);
        std::vector<double> x_p;
        x_p = gauss_omp(L_p, U_p, vec_b_p, n);
        int end_p = clock();
        if (i == 1) {
            show(x_p, n);
            std::cout << std::endl;
            show(x_s, n);
            std::cout << std::endl;
        };
        time_gauss_s[i] = end_s - start_s;
        time_gauss_p[i] = end_p - start_p;
    }
    double sum_gauss_s = 0;
    double sum_gauss_p = 0;
    for (int i = 0; i < time_gauss_s.size(); ++i) {
        sum_gauss_s += time_gauss_s[i];
        sum_gauss_p += time_gauss_p[i];
    }
    std::cout << "Gauss boost: " << sum_gauss_s / sum_gauss_p << std::endl;
    std::cout << std::endl;
// Jacobi test
    std::vector<double> time_jacobi_s(k, 0);
    std::vector<double> time_jacobi_p(k, 0);
    for (int i = 0; i < k; ++i) {
        std::vector<double> x_s(n), x_p(n);
        for (int i = 0; i < n; ++i) {
            x_s[i] = 1;
            x_p[i] = 1;
        }
        int start_s = clock();
        x_s = jacobi(n, matrix_a, vec_b_s, x_s);
        int end_s = clock();
        int start_p = clock();
        x_p = jacobi_omp(n, matrix_a, vec_b_p, x_p);
        int end_p = clock();
        if (i == 1) {
            show(x_p, n);
            std::cout << std::endl;
            show(x_s, n);
            std::cout << std::endl;
        };
        time_jacobi_s[i] = end_s - start_s;
        time_jacobi_p[i] = end_p - start_p;
    }
    double sum_jacobi_s = 0;
    double sum_jacobi_p = 0;
    for (int i = 0; i < time_jacobi_s.size(); ++i) {
        sum_jacobi_s += time_jacobi_s[i];
        sum_jacobi_p += time_jacobi_p[i];
    }
    std::cout << "Jacobi boost: " << sum_jacobi_s / sum_jacobi_p << std::endl;
    std::cout << std::endl;
// Seidel test
    std::vector<double> time_seidel_s(k, 0);
    std::vector<double> time_seidel_p(k, 0);
    for (int i = 0; i < k; ++i) {
        std::vector<double> x_s(n), x_p(n), x_prev_s(n), x_prev_p(n);
        for (int i = 0; i < n; ++i) {
            x_s[i] = 1;
            x_p[i] = 1;
            x_prev_s[i] = 1;
            x_prev_p[i] = 1;
        }
        int start_s = clock();
        x_s = seidel(n, matrix_a, x_s, x_prev_s, vec_b_s);
        int end_s = clock();
        int start_p = clock();
        x_p = seidel_omp(n, matrix_a, x_p, x_prev_p, vec_b_p);
        int end_p = clock();
        if (i == 1) {
            show(x_p, n);
            std::cout << std::endl;
            show(x_s, n);
            std::cout << std::endl;
        };
        time_seidel_s[i] = end_s - start_s;
        time_seidel_p[i] = end_p - start_p;
    }
    double sum_seidel_s = 0;
    double sum_seidel_p = 0;
    for (int i = 0; i < time_seidel_s.size(); ++i) {
        sum_seidel_s += time_seidel_s[i];
        sum_seidel_p += time_seidel_p[i];
    }
    std::cout << "Seidel coeff: " << sum_seidel_s / sum_seidel_p << std::endl;
    std::cout << std::endl;
// average
    std::cout << "Avg Boost: " << ((sum_gauss_s / sum_gauss_p) + (sum_seidel_s / sum_seidel_p)  + (sum_jacobi_s / sum_jacobi_p)) / 3 << std::endl;
}
