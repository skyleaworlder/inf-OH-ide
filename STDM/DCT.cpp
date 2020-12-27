#include "DCT.hpp"

// Quantitum Matrix: Q_{50}
// using Q_{50} as a standard to generate other Matrix
std::vector<double> DCT::Quant_matrix_50 = std::vector<double> {
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109,103,77,
        24, 35, 55, 64, 81, 104,113,92,
        49, 64, 78, 87, 103,121,120,101,
        72, 92, 95, 98, 112,100,103,99
};

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
    const size_t N
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

/*
 * DCT inverse function
 * only able to process SQUARE
 * D: a Compressed Matrix,
 * q_level: a param to specify Quantitum Matrix
 * N: len of D, D must be a square
 */
std::vector<double> DCT::DCT_inverse(
    std::vector<double> D,
    const size_t N,
    size_t q_level
) {
    std::vector<double> C, R;
    std::vector<double> Q { DCT::Quant_matrix_gen(q_level) };

    // generate T and T'
    std::pair<
        std::vector<double>,
        std::vector<double>
    > T_Tp { DCT::T_matrix_gen(N) };
    std::vector<double> T { T_Tp.first }, Tp { T_Tp.second };
    std::vector<double> mult_res { DCT::Mult_square_matrix(
        DCT::Mult_square_matrix(Tp, D), T
    )};

    // round() and unsigned
    std::vector<double> ret;
    for (double elem : mult_res)
        ret.push_back(DCT::round(elem, true));

    return ret;
}

/*
 * round function
 * unsign is true:
 *  input is restricted in [0, 255]
 * while unsign is false:
 *  round() is round() in mathematic.
 */
int DCT::round(double input, bool unsign) {
    if (unsign) {
        if (input > 255)
            return 255;
        else if (input < 0)
            return 0;
        else
            return int(input + 0.5);
    }
    else {
        if (input > 0)
            return int(input + 0.5);
        else
            return int(input - 0.5);
    }
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
        ret.push_back(DCT::round(input));
    }
    return ret;
}

/*
 * return T and Tp
 * once N given, T and Tp will come.
 * using std::pair
 */
std::pair<std::vector<double>, std::vector<double>> DCT::T_matrix_gen(
    const size_t N
) {
    /*
     * generate a 2-dim vector: T
     * to generate T' in accordance with programming requirements.
     */
    std::vector<std::vector<double>> T;
    for (size_t i = 0; i < N; ++i) {
        std::vector<double> T_row;
        for (size_t j = 0; j < N; ++j) {
            if (i == 0)
                T_row.push_back(1.0 / sqrt(N));
            else {
                double theta = ((2*j + 1) * i*M_PI) / (2.0*N);
                T_row.push_back(sqrt(2.0/N) * cos(theta));
            }
        }
        T.push_back(T_row);
    }

    // transpose
    std::vector<double> T_ret, T_tp_ret;
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j) {
            T_ret.push_back(T[i][j]);
            T_tp_ret.push_back(T[j][i]);
        }

    return std::pair<
            std::vector<double>,
            std::vector<double>
        > { T_ret, T_tp_ret };
}

/*
 * Matrix Mult function
 * because C++ stdlib not provide Matrix Class
 * this is a function only process SQUARE!
 *
 * input a and b must have the square number of element.
 */
std::vector<double> DCT::Mult_square_matrix(
    std::vector<double> a,
    std::vector<double> b
) {
    assert(a.size() == b.size());
    std::vector<double> ret;

    size_t len = sqrt(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        const size_t row = i / len;
        const size_t col = i - row * len;
        double to_push = 0;
        for (size_t a_j = 0, b_i = 0; a_j < len && b_i < len; ++a_j, ++b_i)
            to_push += a[row*len + a_j] * b[b_i*len + col];
        ret.push_back(to_push);
    }

    return ret;
}