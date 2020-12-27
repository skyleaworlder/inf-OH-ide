#include "STDM.hpp"

const int STDM::pic_width = 512;
const int STDM::pic_height = 512;
const int STDM::logo_width = 64;
const int STDM::logo_height = 64;
const int STDM::pic_size = 512*512;
const int STDM::logo_size = 64*64;
const int STDM::block_size = 64;

void STDM::imbedAll(double* pic_inp, double* pic_imbed_out, int* logo_inp) {
    const size_t block_byte_num = STDM::block_size * 8;

    size_t cnt = 0;
    for (size_t bias = 0; bias < STDM::pic_size; bias+=STDM::block_size, ++cnt) {
        double* imbed_inp = new double[STDM::block_size];
        double* imbed_out = new double[STDM::block_size];

        // 使用 memcpy_s 是好的，可以使代码简洁（
        memcpy_s(imbed_inp, block_byte_num, (pic_inp+bias), block_byte_num);
        this->imbed(imbed_inp, imbed_out, logo_inp[cnt]);
        memcpy_s((pic_imbed_out+bias), block_byte_num, imbed_out, block_byte_num);

        delete[] imbed_inp;
        delete[] imbed_out;
    }
}

void STDM::imbed(double block_inp[64], double block_out[64], int b, const size_t zig_num) {
    // dct
    std::vector<double> block_vec { array2Vector<double>(block_inp, 64) };
    std::vector<double> block_dct { DCT::DCT(block_vec, 8) };
    std::array<std::array<double, 8>, 8> block_dct_arr { vector2Array(block_dct) };

    // zig-zag
    std::vector<double> zig_zag_vec { getZigZagSlash(block_dct_arr, zig_num) };

    // imbed
    // x_bar: projection of x
    double x_bar, x_sum = 0;
    for (size_t idx = 0; idx < zig_zag_vec.size(); ++idx)
        x_sum += (idx % 2) ? -zig_zag_vec[idx] : zig_zag_vec[idx];
    x_bar = x_sum / zig_zag_vec.size();

    // q_res: q_delta(x - d[b]) + d[b]
    // 这里需要保证 b 一定是 0 或者 1
    // 由于我使用的是原生数组，这里由 assert 保证 b 的取值范围
    double d[2] = { -(this->delta / 4.0), this->delta / 4.0 };
    assert(b == 0 || b == 1);
    double q_res = this->q_delta(this->delta, x_bar - d[b]) + d[b];

    // imbed zig_zag
    // 需要说明的是，这里变量名虽然为 zig-zag，但却不是真正的 zig-zag 表示
    // 只是象征性地取了对角线上的几组数据而已
    for (size_t idx = 0; idx < zig_zag_vec.size(); ++idx)
        zig_zag_vec[idx] += (idx % 2) ? -(q_res - x_bar) : (q_res - x_bar);

    // 将嵌入水印的部分填回原 block 中
    // left half: 包括对角线
    size_t zig_zig_idx = 0;
    for (int zig_beg = zig_num; zig_beg > 0; zig_beg--)
        for (size_t index = 0; index <= block_dct_arr.size()-zig_beg; index++)
            block_dct_arr[index][block_dct_arr.size()-zig_beg-index] = zig_zag_vec[zig_zig_idx++];

    // right half: 从对角线下开始
    for (int zig_beg = 1; zig_beg < zig_num; zig_beg++)
        for (size_t index = zig_beg; index < block_dct_arr.size(); index++)
            block_dct_arr[index][block_dct_arr.size()+zig_beg-1-index] = zig_zag_vec[zig_zig_idx++];

    // dct inverse
    std::vector<double> block_to_dct_inverse { array2Vector(block_dct_arr) };
    std::vector<double> block_imbed { DCT::DCT_inverse(block_to_dct_inverse, 8) };

    vector2Array<double>(block_imbed, block_out);
}

void STDM::decodeAll(double* pic_imbed_inp, int* logo_out) {
    const size_t block_byte_num = block_size * 8;

    size_t cnt = 0;
    for (size_t bias = 0; bias < STDM::pic_size; bias+=STDM::block_size, ++cnt) {
        double* decode_inp = new double[STDM::block_size];
        memcpy_s(decode_inp, block_byte_num, (pic_imbed_inp+bias), block_byte_num);
        // 返回值为 bool 类型，因此 logo_out 还需要进一步类型转换为 u_char
        logo_out[cnt] = this->decode(decode_inp);
        delete[] decode_inp;
    }
}

bool STDM::decode(double block_inp[64], const size_t zig_num) {
    // dct
    std::vector<double> block_vec { array2Vector<double>(block_inp, 64) };
    std::vector<double> block_dct { DCT::DCT(block_vec, 8) };
    std::array<std::array<double, 8>, 8> block_dct_arr { vector2Array(block_dct) };

    // zig-zag
    std::vector<double> zig_zag_vec { getZigZagSlash(block_dct_arr, zig_num) };

    // decode
    double y_bar, y_sum = 0;
    for (size_t idx = 0; idx < zig_zag_vec.size(); ++idx)
        y_sum += (idx % 2) ? -zig_zag_vec[idx] : zig_zag_vec[idx];
    y_bar = y_sum / zig_zag_vec.size();

    // |q_0(Y_bar) - Y_bar| < |q_1(Y_bar) - Y_bar| => b' = 0
    // else b' = 1
    // 都是按照 slides 上的公式走的
    double q_res = this->q_delta(this->delta, y_bar + (this->delta/4.0)) - (this->delta / 4.0);
    return (fabs(q_res - y_bar) >= this->delta / 4.0);
}

double STDM::calcuPfa(int* logo_origin, int* logo_decode) {
    size_t cnt = 0;
    for (size_t idx = 0; idx < STDM::logo_size; ++idx)
        cnt += (logo_origin[idx] != logo_decode[idx]);
    return cnt / static_cast<double>(STDM::logo_size);
}