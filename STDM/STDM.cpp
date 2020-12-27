#include "STDM.hpp"

const int STDM::pic_width = 512;
const int STDM::pic_height = 512;
const int STDM::logo_width = 64;
const int STDM::logo_height = 64;

void STDM::imbedAll(double* pic_inp, double* pic_imbed_out, int* logo_inp) {
    const size_t pic_size = STDM::pic_width * STDM::pic_height;
    const size_t logo_size = STDM::logo_width * STDM::logo_height;
    const size_t block_size = 64;
    const size_t block_byte_num = block_size * 8;

    size_t cnt = 0;
    for (size_t bias = 0; bias < pic_size; bias+=block_size, ++cnt) {
        double* imbed_inp = new double[block_size];
        double* imbed_out = new double[block_size];

        memcpy_s(imbed_inp, block_byte_num, (pic_inp+bias), block_byte_num);
        this->imbed(imbed_inp, imbed_out, logo_inp[cnt]);
        memcpy_s((pic_imbed_out+bias), block_byte_num, imbed_out, block_byte_num);

        delete[] imbed_inp;
        delete[] imbed_out;
    }
}

void STDM::imbed(double block_inp[64], double block_out[64], int b) {
    // dct
    std::vector<double> block_vec { this->_array2Vector<double>(block_inp, 64) };
    std::vector<double> block_dct { DCT::DCT(block_vec, 8) };
    std::array<std::array<double, 8>, 8> block_dct_arr { this->_vector2Array(block_dct) };

    // zig-zag
    const size_t zig_num = 3;
    std::vector<double> zig_zag_vec { this->_getZigZagSlash(block_dct_arr, zig_num) };

    // imbed
    // x_bar: projection of x
    double x_bar, x_sum = 0;
    for (size_t idx = 0; idx < zig_zag_vec.size(); ++idx)
        x_sum += (idx % 2) ? -zig_zag_vec[idx] : zig_zag_vec[idx];
    x_bar = x_sum / zig_zag_vec.size();

    // q_res: q_delta(x - d[b]) + d[b]
    double d[2] = { -(this->delta / 4.0), this->delta / 4.0 };
    double q_res = this->q_delta(this->delta, x_bar - d[b]) + d[b];

    // imbed zig_zag
    for (size_t idx = 0; idx < zig_zag_vec.size(); ++idx)
        zig_zag_vec[idx] += (idx % 2) ? -(q_res - x_bar) : (q_res - x_bar);

    // 将嵌入水印的部分填回原 block 中
    // left half
    size_t zig_zig_idx = 0;
    for (int zig_beg = zig_num; zig_beg > 0; zig_beg--)
        for (size_t index = 0; index <= block_dct_arr.size()-zig_beg; index++)
            block_dct_arr[index][block_dct_arr.size()-zig_beg-index] = zig_zag_vec[zig_zig_idx++];

    // right half
    for (int zig_beg = 1; zig_beg < zig_num; zig_beg++)
        for (size_t index = zig_beg; index < block_dct_arr.size(); index++)
            block_dct_arr[index][block_dct_arr.size()+zig_beg-1-index] = zig_zag_vec[zig_zig_idx++];

    // dct inverse
    std::vector<double> block_to_dct_inverse { this->_array2Vector(block_dct_arr) };
    std::vector<double> block_imbed { DCT::DCT_inverse(block_to_dct_inverse, 8) };

    this->_vector2Array<double>(block_imbed, block_out);
}

void STDM::decodeAll(double* pic_imbed_inp, int* logo_out) {
    const size_t pic_size = STDM::pic_width * STDM::pic_height;
    const size_t logo_size = STDM::logo_width * STDM::logo_height;
    const size_t block_size = 64;
    const size_t block_byte_num = block_size * 8;

    size_t cnt = 0;

    for (size_t bias = 0; bias < pic_size; bias += block_size, ++cnt) {
        double* decode_inp = new double[block_size];
        memcpy_s(decode_inp, block_byte_num, (pic_imbed_inp+bias), block_byte_num);
        logo_out[cnt] = this->decode(decode_inp);
        delete[] decode_inp;
    }
}

bool STDM::decode(double block_inp[64]) {
    // dct
    std::vector<double> block_vec { this->_array2Vector<double>(block_inp, 64) };
    std::vector<double> block_dct { DCT::DCT(block_vec, 8) };
    std::array<std::array<double, 8>, 8> block_dct_arr { this->_vector2Array(block_dct) };

    // zig-zag
    const size_t zig_num = 3;
    std::vector<double> zig_zag_vec { this->_getZigZagSlash(block_dct_arr, zig_num) };

    // decode
    double y_bar, y_sum = 0;
    for (size_t idx = 0; idx < zig_zag_vec.size(); ++idx)
        y_sum += (idx % 2) ? -zig_zag_vec[idx] : zig_zag_vec[idx];
    y_bar = y_sum / zig_zag_vec.size();

    // |q_0(Y_bar) - Y_bar| < |q_1(Y_bar) - Y_bar| => b' = 0
    // else b' = 1
    double d[2] = { -(this->delta / 4.0), this->delta / 4.0 };
    double q_res = this->q_delta(this->delta, y_bar-d[0]) + d[0];
    return (fabs(q_res - y_bar) >= this->delta / 4.0);
}

std::vector<double> STDM::_getZigZagSlash(
    std::array<std::array<double, 8>, 8> input,
    const size_t zig_num
) {
    std::vector<double> zig_zag_arr;
    // 取出需要嵌入水印的部分
    // left half
    for (int zig_beg = zig_num; zig_beg > 0; zig_beg--)
        for (size_t index = 0; index <= input.size()-zig_beg; index++)
            zig_zag_arr.push_back(input[index][input.size()-zig_beg-index]);

    // right half
    // block[index][block.size()+zig_beg-1-index] is very difficult for me to calculate.
    for (int zig_beg = 1; zig_beg < zig_num; zig_beg++)
        for (size_t index = zig_beg; index < input.size(); index++)
            zig_zag_arr.push_back(input[index][input.size()+zig_beg-1-index]);

    return zig_zag_arr;
}

double STDM::calcuPfa(int* logo_origin, int* logo_decode) {
    const size_t logo_size = STDM::logo_width * STDM::logo_height;

    size_t cnt = 0;
    for (size_t idx = 0; idx < logo_size; ++idx)
        cnt += (logo_origin[idx] != logo_decode[idx]);
    return cnt / static_cast<double>(logo_size);
}

void STDM::int2UChar(int* int_inp, u_char* uchar_out) {
    const size_t logo_size = STDM::logo_width * STDM::logo_height;
    const size_t uchar_out_len = logo_size / 8;

    for (size_t idx = 0; idx < uchar_out_len; ++idx) {
        uchar_out[idx] = static_cast<u_char>(0);
        for (int cnt = 7; cnt >= 0; --cnt) {
            // small endian
            bool bit = int_inp[idx*8 + cnt];
            if (bit == 1) {
                u_char c_tmp = 0x01 << (7 - cnt);
                uchar_out[idx] |= c_tmp;
            }
        }
    }
}

std::array<std::array<double, 8>, 8> STDM::_vector2Array(std::vector<double> input) {
    assert(input.size() == 8*8);
    std::array<std::array<double, 8>, 8> to_ret;
    for (size_t index = 0; index < input.size(); index++) {
        size_t row = index / 8;
        size_t col = index - index / 8 * 8;
        to_ret[row][col] = input[index];
    }
    return to_ret;
}

std::vector<double> STDM::_array2Vector(std::array<std::array<double, 8>, 8> input) {
    std::vector<double> to_ret;
    for (size_t row_idx = 0; row_idx < input.size(); row_idx++)
        for (size_t col_idx = 0; col_idx < input[row_idx].size(); col_idx++)
            to_ret.push_back(input[row_idx][col_idx]);
    return to_ret;
}