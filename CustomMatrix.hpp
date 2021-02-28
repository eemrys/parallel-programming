#pragma once

#ifndef CustomMatrix_hpp
#define CustomMatrix_hpp

#include <stdio.h>
#include "CustomVector.hpp"

class CustomMatrix {
    
public:
    int rows = 0;
    std::vector<std::vector<double>> values;
    CustomMatrix() {};
    CustomMatrix(int rows);
    CustomMatrix(std::vector<std::vector<double>> values);
    CustomMatrix operator + (CustomMatrix const &obj);
    CustomMatrix operator * (CustomMatrix const &obj);
    CustomVector operator * (CustomVector const &obj);
    CustomMatrix operator * (double const &obj);
    double frobenius_norm();
    void transpose();
    void print();
};

#endif /* CustomMatrix_hpp */
