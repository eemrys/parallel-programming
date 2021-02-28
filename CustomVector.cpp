#include <iostream>
#include <vector>
#include <omp.h>
#include <random>

#include "CustomVector.hpp"

CustomVector::CustomVector(std::vector<double> values) {
    this->values = values;
    this->size = values.size();
}

CustomVector::CustomVector(int size) {
    this->size = size;
    this->values.resize(this->size);
#pragma omp parallel for
    for (int i = 0; i < size; i++) {
        this->values[i] = 0;
    }
}

CustomVector CustomVector::operator + (CustomVector const &obj) {
    if (this->size != obj.size) {
            throw "vectors have different sizes";
    }
    CustomVector res = CustomVector(this->size);
#pragma omp parallel for
    for (int i = 0; i < res.size; i++) {
        res.values[i] = this->values[i] + obj.values[i];
    }
    return res;
}

CustomVector CustomVector::operator * (double const &obj) {
    CustomVector res = CustomVector(this->size);
#pragma omp parallel for
    for (int i = 0; i < res.size; i++) {
        res.values[i] = this->values[i] * obj;
    }
    return res;
}

double CustomVector::operator * (CustomVector const &obj){
    if (this->size != obj.size) {
            throw "vectors have different sizes";
    }
    double res = 0.0;
#pragma omp parallel for
    for (int i = 0; i < this->size; i++) {
        res += this->values[i] * obj.values[i];
    }
    return res;
}

double CustomVector::norm() {
    double res = 0.0;
#pragma omp parallel for
    for (int i = 0; i < this->size; i++) {
        res += this->values[i] * this->values[i];
    }
    res = std::pow(res, 0.5);
    return res;
}

void CustomVector::print() {
    std::cout << "[ ";
    for (int i = 0; i < this->size - 1; i++) {
        std::cout << this->values[i] << ", ";
    }
    std::cout << this->values[this->size - 1];
    std::cout << " ]" << std::endl;
}
