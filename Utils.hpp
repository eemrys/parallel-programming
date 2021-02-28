#pragma once
#include <vector>
#include <functional>

double euclid_metric(std::vector<double> &a, std::vector<double> &b, int flag);
double max_error(std::vector<double> &a, std::vector<double> &b);
std::vector<std::vector <double> > transpose(std::vector<std::vector<double> > &a);
std::vector<double> actual_vals(std::function <double(double)> &u, double t0, double tm, double h);
std::vector<std::vector<double> > pikar(std::vector<double> &alpha, double t0, double tm, double h, int flag, std::vector<std::function<double(double, std::vector<double> &)> > &func);
