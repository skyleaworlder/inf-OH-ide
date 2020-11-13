#include "DCT.hpp"
#include <iostream>

/*
 * only able to process SQUARE
 * get D Matrix from M(pixel_arr) directly
 * without T and T^T generation
 * (T is an Orthogonal Matrix)
 *
 * In M(pixel_arr), i means column, j means row,
 * so is D Matrix
 */
std::vector<double> DCT::DCT(
    std::vector<double> pixel_arr,
    size_t N
) {
    assert(pixel_arr.size() == N*N);
    std::vector<double> D;

    /*
     * i: column
     * j: row
     *
     * D(i,j) = D: row j column i
     */
    for (size_t j = 0; j < N; ++j)
        for (size_t i = 0; i < N; ++i) {
            double C_i = (i == 0) ? 1.0 / sqrt(2) : 1;
            double C_j = (j == 0) ? 1.0 / sqrt(2) : 1;

            double D_i_j = 0;
            for (size_t y = 0; y < N; ++y)
                for (size_t x = 0; x < N; ++x) {
                    double theta_1 = ((2*x + 1)*i*M_PI) / (2.0*N);
                    double theta_2 = ((2*y + 1)*j*M_PI) / (2.0*N);
                    D_i_j += pixel_arr[y*N + x] * cos(theta_1) * cos(theta_2);
                }
            D_i_j *= (1.0 / sqrt(2*N)) * C_i * C_j;
            D.push_back(D_i_j);
        }

    return D;
}

std::vector<double> DCT::DCT_inverse(
    std::vector<double> pixel_arr,
    size_t N
) {
    assert(pixel_arr.size() == N*N);
    /* TODO: inverse function of DCT */
}

size_t DCT::bound(double input) {
    if (input > 255)
        return 255;
    else if (input < 0)
        return 0;
    else
        return ceil(input);
}

/*
 * the function comes from the highest reply post in:
 * https://stackoverflow.com/questions/29215879/how-can-i-generalize-the-quantization-matrix-in-jpeg-compression
 *
 * through test(test_Q_gen),
 * this function can generate Q Matrix(e.g. Q_{10} and Q_{90}) that given by dct.pdf.
 */
std::vector<double> DCT::Quant_matrix_gen(size_t target_Q) {
    size_t S;
    if (target_Q < 50)
        S = 5000 / target_Q;
    else
        S = 200 - 2*target_Q;

    std::vector<double> ret;
    for (double elem : DCT::Quant_matrix_50) {
        size_t input = floor((S*elem + 50) / 100);
        ret.push_back(DCT::bound(input));
    }
    return ret;
}

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

    vec = DCT::DCT_inverse(vec, 8);
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
}

int main() {
    //test_DCT();
    test_Q_gen();
}