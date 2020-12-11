#include <iostream>
#include <vector>
#include "ASS.hpp"
using namespace SS;

std::vector<std::vector<double>> imbed() {
    return {};
}

void test_return() {
    std::vector<std::vector<double>> arr = imbed();
    std::cout << arr.size() << std::endl;
    for (const auto& elem : arr) {
        for (const auto& flfm : elem)
            std::cout << flfm << " ";
        std::cout << elem.size() << std::endl;
    }
}

void test_zigzag() {
    //std::vector<double> arr = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    block_8_8 array = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64};
    size_t zig_num = 3;
    std::cout << std::endl;
    for (auto arr : array) {
        for (auto elem : arr)
            std::cout << elem << " ";
        std::cout << std::endl;
    }

    std::cout << std::endl;
    array = SS::imbed(array, 1.0, SS::IMBED_B::POS, zig_num);
    for (auto arr : array) {
        for (auto elem : arr)
            std::cout << elem << " ";
        std::cout << std::endl;
    }
    std::cout << SS::decode(array, zig_num);
}

int main() {
    test_zigzag();
}