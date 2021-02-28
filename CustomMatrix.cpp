#include <iostream>
#include <omp.h>
#include <random>
#include <iomanip>

#include "CustomMatrix.hpp"

CustomMatrix::CustomMatrix(int rows) {
    this->rows = rows;
    values.resize(rows);
    //omp_set_nested(1);
    omp_set_max_active_levels(2);
#pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        values[i].resize(rows);
#pragma omp parallel for
        for (int j = 0; j < rows; j++) {
            values[i][j] = 0;
        }
    }
}

CustomMatrix::CustomMatrix(std::vector<std::vector<double>> values) {
    this->values = values;
    rows = values.size();
}

CustomMatrix CustomMatrix::operator + (CustomMatrix const &obj) {
    CustomMatrix res = CustomMatrix(obj.rows);
    //omp_set_nested(1);
    omp_set_max_active_levels(2);
#pragma omp parallel for
    for (int i = 0; i < obj.values.size(); i++) {
#pragma omp parallel for
        for (int j = 0; j < obj.values.size(); j++) {
            res.values[i][j] = this->values[i][j] + obj.values[i][j];
        }
    }
    return res;
}

CustomMatrix CustomMatrix::operator * (double const &obj) {
    CustomMatrix res = CustomMatrix(this->rows);
    //omp_set_nested(1);
    omp_set_max_active_levels(2);
#pragma omp parallel for
    for (int i = 0; i < this->values.size(); i++) {
#pragma omp parallel for
        for (int j = 0; j < this->values.size(); j++) {
            res.values[i][j] = obj * this->values[i][j];
        }
    }
    return res;
}

CustomMatrix CustomMatrix::operator * (CustomMatrix const &obj) {
    if (this->rows != obj.rows) {
        throw "matrices have different sizes";
    }
    CustomMatrix res = CustomMatrix(obj.rows);
    //omp_set_nested(1);
    omp_set_max_active_levels(3);
#pragma omp parallel for
    for (int i = 0; i < this->rows; i++) {
#pragma omp parallel for
        for (int k = 0; k < rows; k++) {
            CustomVector horizontal = CustomVector(this->values[i]);
            CustomVector vertical = CustomVector(this->rows);
#pragma omp parallel for
            for (int j = 0; j < this->rows; j++) {
                vertical.values[j] = obj.values[j][k];
            }
            double current = horizontal * vertical;
            res.values[i][k] = current;
        }
    }
    return res;
}

double CustomMatrix::frobenius_norm() {
    double res = 0;
    //omp_set_nested(1);
    omp_set_max_active_levels(2);
#pragma omp parallel for
    for (int i = 0; i < this->rows; i++) {
#pragma omp parallel for
        for (int j = 0; j < this->rows; j++) {
            res += std::pow(this->values[i][j], 2);
        }
    }
    double output = std::pow(res, 0.5);
    return output;
}

void CustomMatrix::transpose() {
    CustomMatrix a_T = CustomMatrix(this->rows);
    //omp_set_nested(1);
    omp_set_max_active_levels(2);
#pragma omp parallel for
    for (int i = 0; i < this->rows; i++) {
#pragma omp parallel for
        for (int j = 0; j < this->rows; j++) {
            a_T.values[i][j] = this->values[j][i];
        }
    }
    this->values = a_T.values;
}

CustomVector CustomMatrix::operator * (CustomVector const &obj) {
    if (this->rows != obj.size) {
        throw "matrix and vector need to have the same size";
    }
    CustomVector res = CustomVector(obj.size);
    //omp_set_nested(1);
    omp_set_max_active_levels(2);
#pragma omp parallel for
    for (int i = 0; i < res.size; i++) {
#pragma omp parallel for
        for (int j = 0; j < res.size; j++) {
            res.values[i] += this->values[i][j] * obj.values[i];
        }
    }
    return res;
}


void CustomMatrix::print() {
    for (int i = 0; i < this->rows; i++) {
        std::cout << "| ";
        for (int j = 0; j < this->rows; j++) {
            std::cout << std::fixed << std::setprecision(2) << values[i][j] << " ";
        }
        std::cout << "|" << std::endl;
    }
}
