#include <iostream>
#include <fstream>
#include <algorithm>    // min_element & max_element
#include "IG.hpp"
#include "param_est.hpp"
#include "random_gen.hpp"

static void lab_1_1() {
    std::vector<double> arr;
    double miu, sigma;
    size_t sample_num;
    std::cout << "1. now input miu and sigma:(double)" << std::endl;
    std::cin >> miu >> sigma;
    std::cout << "2. input the number of sample dots:(size_t)" << std::endl;
    std::cin >> sample_num;

    std::ofstream out("../data/lab_1_1_data.txt");  // gauss distribute
    out << miu << std::endl << sigma << std::endl << sample_num << std::endl;
    out << std::endl;

    for (size_t index = 0; index < sample_num; ++index)
        arr.push_back(GuassDstrb_single_val(miu, sigma));
    for (double elem : arr)
        out << elem << std::endl;

    std::pair<double, double> res1 = mle_GD(arr);
    std::pair<double, double> res2 = moe_GD(arr);
    std::cout << "using mle_GD and moe_GD to estimate params miu and sigma..." << std::endl;
    std::cout << "3.1 mle_GD result, miu is " << res1.first << " "
            << "sigma is " << res1.second << std::endl;
    out << std::endl;
    out << res1.first << std::endl << res1.second << std::endl;

    std::cout << "3.2 moe_GD result, miu is " << res2.first << " "
            << "sigma is " << res2.second << std::endl;
    out << std::endl;
    out << res2.first << std::endl << res2.second << std::endl;

    std::cout << "    while original miu is " << miu
            << ", and sigma is " << sigma << std::endl;
    out.close();
}

static void lab_1_2() {
    std::vector<double> arr;
    double beta;
    size_t sample_num;
    std::cout << "1. now input beta:(double)" << std::endl;
    std::cin >> beta;
    std::cout << "2. input the number of sample dots:(size_t)" << std::endl;
    std::cin >> sample_num;

    std::ofstream out("../data/lab_1_2_data.txt");
    out << beta << std::endl << sample_num << std::endl;
    out << std::endl;

    for (size_t index = 0; index < sample_num; ++index)
        arr.push_back(ExpDstrb_single_val(beta));
    for (double elem : arr)
        out << elem << std::endl;

    std::cout << "3. mle_EXPD result, beta is " << mle_EXPD(arr) << std::endl
            << "   while original beta is " << beta << std::endl;
    out << std::endl;
    out << mle_EXPD(arr) << std::endl;
    out.close();
}

static void lab_1_3() {
    std::vector<double> arr;
    double beta, c;
    size_t sample_num;
    std::cout << "1. now input beta(scope param)" << std::endl;
    std::cin >> beta;
    std::cout << "2. input the number of sample dots:(size_t)" << std::endl;
    std::cin >> sample_num;

    c = 1.0;
    arr = GGD_vector(beta, c, sample_num);
    std::ofstream out1("../data/lab_1_3_data1.txt");  // GGD 1.0
    out1 << c << std::endl << sample_num << std::endl;
    out1 << std::endl;
    std::cout << "3.1 this is GGD rand result when c = 1.0:" << std::endl;
    for (double elem : arr)
        out1 << elem << std::endl;
    out1.close();

    c = 0.5;
    arr = GGD_vector(beta, c, sample_num);
    std::ofstream out2("../data/lab_1_3_data2.txt"); // GGD 0.5
    out2 << c << std::endl << sample_num << std::endl;
    out2 << std::endl;
    std::cout << "3.2 this is GGD rand result when c = 0.5:" << std::endl;
    for (double elem : arr)
        out2 << elem << std::endl;

    double c_moe = moe_GGD(arr);
    std::cout << "4. moe_GGD result, c is " << c_moe << std::endl
            << "   while original c is " << c << std::endl;
    out2 << std::endl;
    out2 << c_moe << std::endl;
    out2.close();

    double min_ggd05 = *std::min_element(arr.begin(), arr.end());
    double max_ggd05 = *std::max_element(arr.begin(), arr.end());
    std::cout << "5. get theoretical CDF when c = 0.5." << std::endl;
    std::cout << "   please input the sample num of GGD theoretical CDF: ";
    std::cout << "   NOTICE: min is " << min_ggd05 << ", while max is " << max_ggd05 << "." << std::endl;
    std::cin >> sample_num;

    std::pair<
        std::vector<double>,
        std::vector<double>
    > res { GGD_theoretical_CDF(
        beta, c, sample_num,
        min_ggd05, max_ggd05
    )};
    std::ofstream out3("../data/lab_1_3_data3.txt");
    out3 << beta << std::endl;
    out3 << c << std::endl;
    out3 << res.first.size() << std::endl;
    out3 << min_ggd05 << std::endl << max_ggd05 << std::endl;
    out3 << std::endl;

    for (double elem : res.first)
        out3 << elem << std::endl;
    out3 << std::endl;
    for (double elem : res.second)
        out3 << elem << std::endl;
    out3.close();
}

int main() {
    size_t choice;
    std::cout << "please input your choice: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            lab_1_1();
            break;
        case 2:
            lab_1_2();
            break;
        case 3:
            lab_1_3();
            break;
        default:
            break;
    }
}