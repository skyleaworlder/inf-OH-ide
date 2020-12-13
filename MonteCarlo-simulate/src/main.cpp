#include "random_gen.hpp"
#include "iefrc.hpp"
#include "MonteCarlo.hpp"

#include <iostream>
#include <vector>
#include <fstream>

/**
 * @brief 最终的测试函数
 * @param func 函数指针，用来生成 GGD 分布的
 * @param a 嵌入强度
 * @param STD 标准差
 * @param N 样本数量
 */
void lab_2_2_1(
    std::vector<double>(*func)(const double size),
    std::ostream& out,
    double a=1.8,
    size_t iter_num=100000,
    double STD=10.0,
    size_t N=1000
) {

    // 理论误检率
    const std::vector<double> pfa_theory_arr {
        1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1
    };

    std::vector<double> psi_theory_arr;
    std::vector<double> pm_theory_arr;
    for (const double& pfa_theory : pfa_theory_arr) {
        // 根据理论 sigma 和理论 pfa 计算理论阈值
        psi_theory_arr.push_back(MC::calcuPsiTheory(STD, pfa_theory, N));
        // 计算理论漏检率
        pm_theory_arr.push_back(MC::calcuPmTheory(pfa_theory, N, a, STD));
    }

    // 嵌入强度 a = 1.8
    std::vector<double> experiment_pm_dot;
    std::vector<double> experiment_pfa_dot;
    // 7 个太慢了，现在只是 6 个
    for (size_t i = 0; i < pfa_theory_arr.size(); i++) {
        size_t pm_cnt = 0;
        size_t pfa_cnt = 0;

        // test_cnt < 1e6，因为题目要求误差在 1e-6 以下
        // 但是 1e6 太慢了
        for (size_t j = 0; j < iter_num; j++) {
            std::vector<double> X_MtClo { func(N) };

            // 初始化 MonteCarlo 对象
            MC::MonteCarlo MtClo(X_MtClo);
            MtClo.imbed(a);
            MtClo.genLinearCorrelator();

            // 根据实验值进行检验
            pm_cnt += MtClo.isMiss(psi_theory_arr[i]);
            pfa_cnt += MtClo.isFalseAlarm(psi_theory_arr[i]);
            if (j % 1000 == 0) {
                out << "cnt: " << j << " ";
                out << "psi_theory_arr: " << psi_theory_arr[i] << " ";
                out << "LS1 / LS0: " << MtClo.L_S_H1 << "/" << MtClo.L_S_H0<< " ";
                out << "\n";
            }
        }

        out << "experimental:" << std::endl;
        out << "(" << pm_cnt << ", " << iter_num << ")" << std::endl;
        out << "(" << pfa_cnt << ", " << iter_num << ")" << std::endl;

        experiment_pm_dot.push_back(pm_cnt / static_cast<double>(iter_num));
        experiment_pfa_dot.push_back(pfa_cnt / static_cast<double>(iter_num));
    }


    out << "thoeritical:" << std::endl;
    out << "pm: ";
    for (const double& elem : pm_theory_arr)
        out << elem << " ";
    out << '\n';
    out << "pfa: ";
    for (const double& elem : pfa_theory_arr)
        out << elem << " ";
    out << '\n';
    out << "psi_theory_arr: ";
    for (const double& elem : psi_theory_arr)
        out << elem << " ";
    out << '\n';

    out << "pm, when a = " << a << ":" << std::endl;
    for (const double& elem : experiment_pm_dot)
        out << elem << " ";
    out << std::endl;

    out << "pfa, when a = " << a << ":" << std::endl;
    for (const double& elem : experiment_pfa_dot)
        out << elem << " ";
    out << std::endl;

}

void lab_2_2_2(
    std::ostream& out,
    const double STD_voice,
    const size_t iter_num=10000
) {
    const double STD = 10;
    const double mean_voice = 0;
    const double a = 1.8;
    const double N = 1000;

    // 理论误检率
    const std::vector<double> pfa_theory_arr {
        1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1
    };

    std::vector<double> psi_theory_arr;
    std::vector<double> pm_theory_arr;

    std::cout << "STD_voice = " << STD_voice << ":" << std::endl;
    // 通过理论误检率计算理论阈值
    for (const double& pfa_theory : pfa_theory_arr)
        psi_theory_arr.push_back(MC::calcuPsiTheory(STD, pfa_theory, N));

    // 通过理论阈值求漏检率
    for (const double& pfa_theory : pfa_theory_arr)
        pm_theory_arr.push_back(MC::calcuPmTheory(pfa_theory, N, a, sqrt(
            STD*STD+STD_voice*STD_voice
        )));

    std::vector<double> experiment_pm_dot;
    std::vector<double> experiment_pfa_dot;
    for (size_t level = 0; level < pfa_theory_arr.size(); level++) {
        size_t pm_cnt = 0;
        size_t pfa_cnt = 0;

        for (size_t index = 0; index < iter_num; index++) {
            // 生成宿主容器
            std::vector<double> GD_host { GD_vector(mean_voice, STD, N) };

            // 初始化 MonteCarlo 对象
            MC::MonteCarlo GD_MtClo(GD_host);
            GD_MtClo.imbed(a);
            GD_MtClo.addVoice(STD_voice);
            GD_MtClo.genLinearCorrelator();

            // 根据实验值进行检验
            pm_cnt += GD_MtClo.isMiss(psi_theory_arr[level]);
            pfa_cnt += GD_MtClo.isFalseAlarm(psi_theory_arr[level]);

            if (index % 1000 == 0) {
                out << "cnt: " << index << " ";
                out << "psi_theory_arr: " << psi_theory_arr[index] << " ";
                out << "LS1 / LS0: " << GD_MtClo.L_S_H1 << "/" << GD_MtClo.L_S_H0<< " ";
                out << "\n";
            }
        }

        out << "experimental:" << std::endl;
        out << "(" << pm_cnt << ", " << iter_num << ")" << std::endl;
        out << "(" << pfa_cnt << ", " << iter_num << ")" << std::endl;

        experiment_pm_dot.push_back(pm_cnt / static_cast<double>(iter_num));
        experiment_pfa_dot.push_back(pfa_cnt / static_cast<double>(iter_num));
    }

    out << "thoeritical:" << std::endl;
    out << "pm:";
    for (const double& elem : pm_theory_arr)
        out << elem << " ";
    out << '\n';
    out << "pfa:";
    for (const double& elem : pfa_theory_arr)
        out << elem << " ";
    out << '\n';
    out << "psi_theory_arr: ";
    for (const double& elem : psi_theory_arr)
        out << elem << " ";
    out << '\n';

    out << "pm, when STD = " << STD << ":" << std::endl;
    for (const double& elem : experiment_pm_dot)
        out << elem << " ";
    out << std::endl;

    out << "pfa, when STD = " << STD << ":" << std::endl;
    for (const double& elem : experiment_pfa_dot)
        out << elem << " ";
    out << std::endl;
}

void lab_2_2_1_utils() {
    std::ofstream out_c20_a18("../data/c20_a18.txt");
    std::ofstream out_c10_a18("../data/c10_a18.txt");
    std::ofstream out_c20_a15("../data/c20_a15.txt");
    std::ofstream out_c05_a18("../data/c05_a18.txt");
    lab_2_2_1(GGD20, out_c20_a18);
    lab_2_2_1(GGD10, out_c10_a18);
    lab_2_2_1(GGD20, out_c20_a15, 1.5);
    lab_2_2_1(GGD05, out_c05_a18);
    out_c05_a18.close();
    out_c10_a18.close();
    out_c20_a18.close();
    out_c20_a15.close();
}

void lab_2_2_2_utils() {
    std::ofstream out_gd_a18_v25("../data/gd_a18_v25.txt");
    std::ofstream out_gd_a18_v50("../data/gd_a18_v50.txt");
    std::ofstream out_gd_a18_v0("../data/gd_a18_v0.txt");
    lab_2_2_2(out_gd_a18_v25, 2.5);
    lab_2_2_2(out_gd_a18_v50, 5.0);
    lab_2_2_2(out_gd_a18_v0, 0.0);
    out_gd_a18_v50.close();
    out_gd_a18_v25.close();
    out_gd_a18_v0.close();
}

#ifndef DEBUG_TEST

int main() {
    //lab_2_2_1_utils();
    //lab_2_2_2_utils();
}

#endif // !DEBUG_TEST