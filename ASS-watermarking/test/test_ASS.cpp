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
    std::vector<double> arr = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    for (auto elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    std::vector<double> zig = _zig_zag(arr);
    for (auto elem : zig) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    zig = _getSlashFromZigZag(zig, 2);
    for (auto elem : zig) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    arr = _imbedSS2Block(arr, zig);
}

int main() {
    test_zigzag();
}