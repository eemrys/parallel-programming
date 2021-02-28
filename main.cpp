#include <iostream>
#include <random>

#include "CustomVector.hpp"
#include "CustomMatrix.hpp"

// единичная матрица для тестирования умножения матриц
CustomMatrix create_E_mat(int rows) {
    CustomMatrix res = CustomMatrix(rows);
    for (int i = 0; i < rows; i++) {
        res.values[i][i] = 1;
    }
    return res;
}

std::vector<double> create_rand_vec(int rows) {
    std::vector<double> v;
    for (int i = 0; i < rows; i++) {
        double curr = rand() % 100;
        curr /= 100;
        v.push_back(curr);
    }
    return v;
}

std::vector<std::vector<double>> create_rand_mat(int n) {
    std::vector<std::vector<double>> mat;
    for (int i = 0; i < n; i++) {
        mat.push_back({});
        for (int j = 0; j < n; j++)
        {
            double curr = rand() % 100;
            curr /= 100;
            mat[i].push_back(curr);
        }
    }
    return mat;
}


int main() {
    setlocale(LC_ALL, "Russian");
    
    std::cout << "1. Вектор" << std::endl;
    std::cout << "2. Матрица" << std::endl;
    std::cout << "0. Выход" << std::endl;
    bool cont = true;
    while (cont) {
        int cmd;
        std::cout << "Введите номер команды: ";
        std::cin >> cmd;
        switch (cmd) {
            case 0:
                cont = false;
                break;
            case 1: {
                CustomVector a = CustomVector(create_rand_vec(4));
                CustomVector b = CustomVector(create_rand_vec(4));
                std::cout << "a:" << std::endl;
                a.print();
                std::cout << "b:" << std::endl;
                b.print();
                try {
                    CustomVector c = a + b;
                    std::cout << "Сумма:" << std::endl;
                    c.print();
                }
                catch (const char *msg) {
                    std::cerr << msg << std::endl;
                }
                
                try {
                    double d = a * b;
                    std::cout << "Скалярное произв.:" << std::endl;
                    std::cout << d << std::endl;
                }
                catch (const char *msg) {
                    std::cerr << msg << std::endl;
                }
                
                std::cout << "Длина a:" << std::endl;
                std::cout << a.norm() << std::endl;

                a = a * 10;
                std::cout << "Умножение a на 10:" << std::endl;
                a.print();
                
                break;
            }
            case 2: {
                CustomMatrix A = CustomMatrix(create_rand_mat(5));
                CustomMatrix B = CustomMatrix(create_rand_mat(5));
                std::cout << "A:" << std::endl;
                A.print();
                std::cout << "B:" << std::endl;
                B.print();
                
                A.transpose();
                std::cout << "Транспонирование:" << std::endl;
                A.print();
                
                try {
                    CustomMatrix C = A + B;
                    std::cout << "Сумма:" << std::endl;
                    C.print();
                }
                catch (const char *msg) {
                    std::cerr << msg << std::endl;
                }
                
                CustomMatrix E = create_E_mat(5);
                std::cout << "E:" << std::endl;
                E.print();
                try {
                    CustomMatrix D = A * E;
                    std::cout << "A * E:" << std::endl;
                    D.print();
                }
                catch (const char *msg) {
                    std::cerr << msg << std::endl;
                }
                
                std::cout << "Норма Фробениуса:" << std::endl;
                std::cout << A.frobenius_norm() << std::endl;

                A = A * 10;
                std::cout << "Умножение на 10:" << std::endl;
                A.print();

                CustomVector vec = CustomVector(create_rand_vec(5));
                std::cout << "Vec:" << std::endl;
                vec.print();
                try {
                    CustomVector res = A * vec;
                    std::cout << "Умножение на вектор:" << std::endl;
                    res.print();
                }
                catch (const char *msg) {
                    std::cerr << msg << std::endl;
                }
                
                break;
            }
            default:
                std::cout << "Некорректный номер команды" <<  std::endl;
        }
    }
}
