#include "Functions.hpp"

double u1(double t) {
    return t*t + 2*t + 1;
}
double u2(double t) {							
    return t;
}
double u3(double t) {							
    return log(t) + t*t;
}
double u4(double t) {							
    return sin(t);
}
double u5(double t) {							
    return cos(t);
}

double f1(double t, std::vector<double> &u) {
    return 2 * u[1] + 2;
}
double f2(double t, std::vector<double> &u) {
    return u[3] * u[3] + u[4] * u[4];
}
double f3(double t, std::vector<double> &u) {
    return 1 / u[1] + 2 * u[1];
}
double f4(double t, std::vector<double> &u) {
    return u[4];
}
double f5(double t, std::vector<double> &u) {
    return -u[3];
}
