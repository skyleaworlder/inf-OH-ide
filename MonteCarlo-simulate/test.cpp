#include "random_gen.hpp"
#include "iefrc.hpp"
#include "MonteCarlo.hpp"

#include <iostream>
#include <vector>
using namespace std;

void test_imbed() {
    vector<double> X { GGD20(10) };
    MC::MonteCarlo tmp(X);
    tmp.imbed(2.3);
    tmp.genLinearCorrelator();
    for (double elem : tmp.S_H0)
        cout << elem << " ";
    cout << endl;
    for (double elem : tmp.S_H1)
        cout << elem << " ";
    cout << endl;
    cout << tmp.L_S_H0 << " " << tmp.L_S_H1 << endl;
}

void test_variance() {
    double STD = 10;
    double c = 4;
    vector<double> X {GGD20(1000)};
    std::cout << MC::mean(X) << '\n';
    std::cout << MC::variance(X);
}

void test_Q() {
    double q_res = MC::Q(0.4);
    double iq_res = MC::iQ(0.4);
    cout << q_res << " " << iq_res << endl;
    std::cout << MC::Q(MC::iQ(0.4)) << '\n';
    std::cout << MC::iQ(MC::Q(0.4));
}

void test_pfa_pm() {
    // 均值
    double mean = 0;
    // 标准差
    double STD = 10.0;

    // 嵌入强度
    double a = 1.8;
    // 每次的样本数量
    size_t N = 1000;

    const std::vector<double> pfa_theory_arr {
        1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1
    };
    std::vector<double> psi_c_2_0;
    std::vector<double> pm_c_2_0;

    for (const double& pfa_theory : pfa_theory_arr) {
        std::cout << "pfa_theory: " << pfa_theory << std::endl;
        psi_c_2_0.push_back(MC::calcuPsiTheory(STD, pfa_theory, N));
    }
    for (size_t index = 0; index < pfa_theory_arr.size(); index++) {
        pm_c_2_0.push_back(MC::calcuPmTheory(pfa_theory_arr[index], N, a, STD));
    }

    for (double elem : pfa_theory_arr)
        cout << elem << " ";
    for (double elem : pm_c_2_0)
        cout << elem << " ";
}

#ifdef DEBUG_TEST

int main() {
    //test_imbed();
    //test_variance();
    //test_Q();
    test_pfa_pm();
}

#endif //DEBUG_TEST