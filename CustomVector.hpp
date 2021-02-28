#pragma once

#ifndef CustomVector_hpp
#define CustomVector_hpp

#include <stdio.h>
#include <vector>

class CustomVector {

public:
    int size = 0;
    std::vector<double> values;
    CustomVector() {};
    CustomVector(std::vector<double> values);
    CustomVector(int size);
    CustomVector operator + (CustomVector const &obj);
    CustomVector operator * (double const &obj);
    double operator * (CustomVector const &obj);
    double norm();
    void print();
};

#endif /* CustomVector_hpp */
