#pragma once
#ifndef gauss_hpp
#define gauss_hpp

#include <stdio.h>
#include <vector>

void LU(std::vector<std::vector<double> > A, std::vector<std::vector<double> > &L,
        std::vector<std::vector<double> > &U, int n);
void LU_omp(std::vector<std::vector<double> > A, std::vector<std::vector<double> > &L,
            std::vector<std::vector<double> > &U, int n);
std::vector<double> gauss(std::vector<std::vector<double> > L, std::vector<std::vector<double> > U, std::vector<double> b, int n);
std::vector<double> gauss_omp(std::vector<std::vector<double> > L, std::vector<std::vector<double> > U, std::vector<double> b, int n);

#endif /* gauss_hpp */
