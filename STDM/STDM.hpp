#include "DCT.hpp"

#include <iostream>
#include <Windows.h>
#include <vector>
#include <array>
#include <cassert>
#include <algorithm>
#include <cmath>

class STDM {

public:
    static const int pic_width;
    static const int pic_height;
    static const int logo_width;
    static const int logo_height;

    // 就是那个三角
    double delta;

public:
    STDM(double delta): delta(delta) {};
    ~STDM() {};
    void imbedAll(double* pic_inp, double* pic_imbed_out, int* logo_inp);
    void imbed(double block_inp[64], double block_out[64], int b);
    void decodeAll(double* pic_imbed_inp, int* logo_out);
    bool decode(double block_inp[64]);

    double calcuPfa(int* logo_origin, int* logo_decode);

    inline double q_delta(const double delta, const double x) {
        return delta * floor(x / delta + 0.5);
    }

    void int2UChar(int* int_inp, u_char* uchar_out);

    template<typename T>
    void _vector2Array(std::vector<T> input, T* output) {
        for (size_t idx = 0; idx < input.size(); idx++)
            output[idx] = input[idx];
    }

    template<typename T>
    std::vector<T> _array2Vector(T* input, const size_t size) {
        std::vector<T> to_ret;
        for (size_t idx = 0; idx < size; idx++)
            to_ret.push_back(input[idx]);
        return to_ret;
    }

    std::vector<double> _getZigZagSlash(std::array<std::array<double, 8>, 8> input, const size_t zig_num);
    std::array<std::array<double, 8>, 8> _vector2Array(std::vector<double> input);
    std::vector<double> _array2Vector(std::array<std::array<double, 8>, 8> input);
};