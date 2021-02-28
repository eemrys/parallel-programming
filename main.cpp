#include <iostream>
#include <ctime>
#include <iomanip>

#include "Utils.hpp"
#include "Functions.hpp"

int main() {
    double t0 = 1;
    double tm = 5;
    double h = 0.001;

    std::vector<std::function<double(double, std::vector<double>&)> > func {f1, f2, f3, f4, f5};
    std::vector<double> alpha {u1(t0), u2(t0), u3(t0), u4(t0), u5(t0)};
    std::vector<std::function<double(const double)> > u {u1, u2, u3, u4, u5};
    std::vector<double> vals;

    std::vector<std::vector<double> > sequential(pikar(alpha, t0, tm, h, 0, func));
    std::vector<std::vector<double> > parallel(pikar(alpha, t0, tm, h, 1, func));
    std::vector<std::vector<double> > sequential_T(transpose(sequential));
    std::vector<std::vector<double> > parallel_T(transpose(parallel));
   
    for (int i = 0; i < 5; ++i) {
        vals = actual_vals(u[i], t0, tm, h);
        std::cout << std::fixed << std::setprecision(5) << max_error(vals, sequential_T[i]) << " " << max_error(vals, parallel_T[i]) << std::endl;
    }

    double sum_seq = 0;
    double sum_par = 0;
    double start, end;
    
    for (int i = 0; i < 30; ++i ) {
        start = clock();
        pikar(alpha, t0, tm, h, 0, func);
        end = clock();
        
        sum_seq += (end - start);

        start = clock();
        pikar(alpha, t0, tm, h, 1, func);
        end = clock();

        sum_par += (end - start);
    }
    std::cout << "Boost coeff: " << sum_seq / sum_par << std::endl;
}
