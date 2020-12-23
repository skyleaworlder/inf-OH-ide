#include "DCT.hpp"
#include <iostream>
using namespace DCT;

/*
 * test part
 */
void test_DCT() {
    /*
     * both test data and algothrim from "Image Compression and the Discrete Cosine Transform",
     * which written by Ken Cabeen and Peter Gent.
     */
    std::vector<double> vec {
        26, -5, -5, -5, -5, -5, -5, 8,
        64, 52, 8, 26, 26, 26, 8, -18,
        126, 70 ,26, 26, 52, 26, -5, -5,
        111, 52, 8, 52, 52, 38, -5, -5,
        52, 26, 8, 39, 38, 21, 8, 8,
        0, 8, -5, 8, 26, 52, 70, 26,
        -5, -23, -18, 21, 8, 8, 52, 38,
        -18, 8, -5, -5, -5, 8, 26, 8
    };
    vec = DCT::DCT(vec, 8);
    for (double elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    vec = DCT::DCT_inverse(vec, 8, 50);
    for (double elem : vec) {
        std::cout << elem << " ";
    }
}

void test_Q_gen() {
    std::vector<double> vec { DCT::Quant_matrix_gen(90) };
    for (double elem : vec)
        std::cout << elem << " ";
    std::cout << std::endl;

    vec = DCT::Quant_matrix_gen(10);
    for (double elem : vec)
        std::cout << elem << " ";
    std::cout << std::endl;

    vec = DCT::Quant_matrix_gen(50);
    for (double elem : vec)
        std::cout << elem << " ";
    std::cout << std::endl;
}

// test well
void test_T_transpose() {
    std::pair<std::vector<double>, std::vector<double>> vec { DCT::T_matrix_gen(8) };
    std::vector<double> T { vec.first };
    std::vector<double> T_tp { vec.second };

    for (double elem : T)
        std::cout << elem << " ";
    std::cout << std::endl;
    for (double elem : T_tp)
        std::cout << elem << " ";
}

// test well
void test_mult() {
    std::vector<double> a {4,2,1,4};
    std::vector<double> b {3,5,8,7};
    a = DCT::Mult_square_matrix(a,b);
    for (double elem : a)
        std::cout << elem << " ";
}

int main() {
    test_DCT();
    //test_Q_gen();
    //test_T_transpose();
    //test_mult();
}