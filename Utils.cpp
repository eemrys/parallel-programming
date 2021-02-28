#include <cmath>
#include <omp.h>
#include <cstdint>
#include <iostream>

#include "Utils.hpp"

double euclid_metric(std::vector<double> &a, std::vector<double> &b, int flag) {
    double e = 0;
#pragma omp parallel for reduction(+:e) if (flag)
    for (int i = 0; i < int(a.size()); ++i) {
        e += ((a[i] - b[i]) * (a[i] - b[i]));
    }
    return sqrt(e);
}

std::vector<double> actual_vals(std::function<double(double)> &u, double t0, double tm, double h) {
    std::vector<double> vals;
    double start = t0;
    vals.push_back(u(t0));
    while (start < tm) {
        start += h;
        vals.push_back(u(start));
    }
    return vals;
}

std::vector<std::vector<double> > pikar(std::vector<double> &alpha, double t0, double tm, double h, int flag, std::vector<std::function<double(double, std::vector<double>&)> > &Func) {
    
    double eps = 0.00001;

    std::vector<double> t_vals;
    double t_curr = t0;
    t_vals.push_back(t_curr);
    while (t_curr < tm) {
        t_curr += h;
        t_vals.push_back(t_curr);
    }

    std::vector<std::vector<double> > u(t_vals.size(), alpha);

    int thread_num;
    if (flag) {
        thread_num = omp_get_num_procs();
    }
    else {
        thread_num = 1;
    }

    int pts_num = int(t_vals.size()) / thread_num;
    std::vector<std::vector<double> > u_vals(t_vals.size());
    std::vector<std::vector<double> > i_vals(t_vals.size() - 1);
    std::vector<std::vector<double> > i_sum(thread_num);

#pragma omp parallel for if (flag)
    for (int i = 0; i < int(i_vals.size()); ++i) {
        i_vals[i].resize(Func.size(), 0.0);
    }

#pragma omp parallel for if (flag)
    for (int i = 0; i < thread_num; ++i) {
        i_sum[i].resize(Func.size(), 0.0);
    }

    bool cont = true;

#pragma omp parallel num_threads(thread_num)
    {
        while (cont) {
#pragma omp single nowait
            {
                u_vals = u;
            }
            int curr_thread = omp_get_thread_num();
            int first = curr_thread * pts_num;
            int last = first + pts_num;
            
            if (curr_thread == thread_num - 1) {
                last = int(t_vals.size()) - 1;
            }
            
            for (int i = 0; i < Func.size(); ++i) {
                i_sum[curr_thread][i] = 0;
            }

            for (int i = first; i < last; ++i) {
                for (int j = 0; j < Func.size(); ++j) {
                    i_vals[i][j] = (t_vals[i + 1] - t_vals[i]) * (Func[j](t_vals[i], u[i]) + Func[j](t_vals[i + 1], u[i + 1])) / 2;
                    i_sum[curr_thread][j] += i_vals[i][j];
                }
            }

            if (curr_thread < (thread_num - 1)) {
                last = last - 1;
            }
            
#pragma omp barrier
            
            for (int i = 0; i < Func.size(); ++i) {
                u[first][i] = u[0][i];
                for (int j = 0; j < curr_thread; ++j) {
                    u[first][i] += i_sum[j][i];
                }
            }
            
            for (int i = first; i < last;++i) {
                for (int j = 0; j < Func.size(); ++j) {
                    u[i + 1][j] = u[i][j] + i_vals[i][j];
                }
            }
            
#pragma omp single
            
            {
                int i = 0;
                double d = euclid_metric(u[i], u_vals[i], flag);
                while (((i+1) < t_vals.size()) && (d < eps)) {
                    i++;
                    d = euclid_metric(u[i], u_vals[i], flag);
                }
                if (d < eps) {
                    cont = false;
                }
            }

#pragma omp barrier
            
        }
    }
    return u;
}

double max_error(std::vector<double> &a, std::vector<double> &b) {
    double ma = 0, v = 0;
    for (int i = 0; i < a.size(); ++i) {
        v = fabs(a[i] - b[i]);
        if (v > ma) {
            ma = v;
        }
    }
    return ma;
}

std::vector<std::vector<double> > transpose(std::vector<std::vector<double> > &a) {
    std::vector<std::vector<double> > a_T(a[0].size(), std::vector<double>(a.size()));
    for (int i = 0; i < a[0].size(); ++i) {
        for (int j = 0; j < a.size(); ++j) {
            a_T[i][j] = a[j][i];
        }
    }
    return a_T;
}
