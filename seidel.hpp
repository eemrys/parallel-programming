#pragma once
#ifndef seidel_hpp
#define seidel_hpp

#include <vector>
#include <stdio.h>

bool converge(std::vector<double> x, std::vector<double> x_next, int n, double eps);
std::vector<double> seidel(int n, std::vector<std::vector<double> > a, std::vector<double> x, std::vector<double> x_prev, std::vector<double> b);
std::vector<double> seidel_omp(int n, std::vector<std::vector<double> > a, std::vector<double> x, std::vector<double> p, std::vector<double> b);
#endif /* seidel_hpp */
