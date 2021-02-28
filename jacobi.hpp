#pragma once
#ifndef jacobi_hpp
#define jacobi_hpp

#include <stdio.h>
#include <vector>

std::vector<double> jacobi(int n, std::vector<std::vector<double> > matrix_a, std::vector<double> b, std::vector<double> x);
std::vector<double> jacobi_omp(int n, std::vector<std::vector<double> > matrix_a, std::vector<double> b, std::vector<double> x);

#endif /* jacobi_hpp */
