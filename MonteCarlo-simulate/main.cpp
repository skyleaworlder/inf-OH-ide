#include "random_gen.hpp"
#include "iefrc.hpp"
#include "montecarlo.hpp"

#include <iostream>
#include <vector>

void lab_2_2_1() {
    // 均值
    double mean = 0;
    // 标准差
    double STD = 10.0;

    // 嵌入强度
    double a = 1.8;
    // 每次的样本数量
    size_t N = 1000;

    // 理论误检率
    const std::vector<double> pfa_theory_arr {
        1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1
    };

    // 根据理论 sigma 和理论 pfa 计算理论阈值
    std::vector<double> psi_c_2_0;
    std::vector<double> psi_c_1_0;
    std::vector<double> psi_c_0_5;
    for (const double& pfa_theory : pfa_theory_arr) {
        psi_c_2_0.push_back(MC::calcuPsiTheory(STD, pfa_theory, N));
        psi_c_1_0.push_back(MC::calcuPsiTheory(STD, pfa_theory, N));
        psi_c_0_5.push_back(MC::calcuPsiTheory(STD, pfa_theory, N));
    }

    // 计算理论漏检率
    std::vector<double> pm_c_2_0;
    std::vector<double> pm_c_1_0;
    std::vector<double> pm_c_0_5;
    for (const double& pfa_theory : pfa_theory_arr) {
        pm_c_2_0.push_back(MC::calcuPmTheory(pfa_theory, N, a, STD));
        pm_c_1_0.push_back(MC::calcuPmTheory(pfa_theory, N, a, STD));
        pm_c_0_5.push_back(MC::calcuPmTheory(pfa_theory, N, a, STD));
    }

    // 无攻击情况，c = 2.0
    // 嵌入强度 a = 1.8
    std::cout << "c = 2.0:\n";
    std::vector<double> experiment_pm_dot_2_0;
    std::vector<double> experiment_pfa_dot_2_0;
    // 7 个太慢了，现在只是 6 个
    for (size_t level = 0; level < pfa_theory_arr.size(); level++) {
        const double a = 1.8;
        const size_t N = 1000;
        const size_t iter_num = 100000;
        size_t pm_cnt = 0;
        size_t pfa_cnt = 0;

        // test_cnt < 1e6，因为题目要求误差在 1e-6 以下
        // 但是 1e6 太慢了
        for (size_t test_cnt = 0; test_cnt < iter_num; test_cnt++) {
            std::vector<double> X_c_2_0 { GGD20(N) };

            // 初始化 MonteCarlo 对象
            MC::MonteCarlo c_2_0(X_c_2_0);
            c_2_0.imbed(a);
            c_2_0.genLinearCorrelator();

            // 根据实验值进行检验
            pm_cnt += c_2_0.isMiss(psi_c_2_0[level]);
            pfa_cnt += c_2_0.isFalseAlarm(psi_c_2_0[level]);
            if (test_cnt % 1000 == 0) {
                std::cout << "cnt: " << test_cnt << " ";
                std::cout << "psi_c_2_0: " << psi_c_2_0[level] << " ";
                std::cout << "LS1 / LS0: " << c_2_0.L_S_H1 << "/" << c_2_0.L_S_H0<< " ";
                std::cout << "\n";
            }
        }
        /*
        std::cout << "experimental:" << std::endl;
        std::cout << "(" << pm_cnt << ", " << iter_num << ")" << std::endl;
        std::cout << "(" << pfa_cnt << ", " << iter_num << ")" << std::endl;
        */
        experiment_pm_dot_2_0.push_back(pm_cnt / static_cast<double>(iter_num));
        experiment_pfa_dot_2_0.push_back(pfa_cnt / static_cast<double>(iter_num));
    }

    /*
    std::cout << "thoeritical:" << std::endl;
    std::cout << "pm: ";
    for (const double& elem : pm_c_2_0)
        std::cout << elem << " ";
    std::cout << '\n';
    std::cout << "pfa: ";
    for (const double& elem : pfa_theory_arr)
        std::cout << elem << " ";
    std::cout << '\n';
    std::cout << "psi: ";
    for (const double& elem : psi_c_2_0)
        std::cout << elem << " ";
    std::cout << '\n';

    std::cout << "pm, when c = 2.0 / a = 1.8:" << std::endl;
    for (const double& elem : experiment_pm_dot_2_0)
        std::cout << elem << " ";
    std::cout << std::endl;

    std::cout << "pfa, when c = 2.0 / a = 1.8:" << std::endl;
    for (const double& elem : experiment_pfa_dot_2_0)
        std::cout << elem << " ";
    std::cout << std::endl;
    */

    // 无攻击情况，c = 1.0
    // 嵌入强度 a = 1.8
    std::cout << "c = 1.0:\n";
    std::vector<double> experiment_pm_dot_1_0;
    std::vector<double> experiment_pfa_dot_1_0;
    for (size_t level = 0; level < pfa_theory_arr.size(); level++) {
        const double a = 1.8;
        const size_t N = 1000;
        const size_t iter_num = 100000;
        size_t pm_cnt = 0;
        size_t pfa_cnt = 0;

        for (size_t test_cnt = 0; test_cnt < iter_num; test_cnt++) {
            std::vector<double> X_c_1_0 { GGD10(N) };

            // 初始化 MonteCarlo 对象
            MC::MonteCarlo c_1_0(X_c_1_0);
            c_1_0.imbed(a);
            c_1_0.genLinearCorrelator();

            // 根据实验值进行检验
            pm_cnt += c_1_0.isMiss(psi_c_1_0[level]);
            pfa_cnt += c_1_0.isFalseAlarm(psi_c_1_0[level]);
        }
        experiment_pm_dot_1_0.push_back(pm_cnt / static_cast<double>(iter_num));
        experiment_pfa_dot_1_0.push_back(pfa_cnt / static_cast<double>(iter_num));
    }

    // 无攻击情况，c = 0.5
    // 嵌入强度 a = 1.8
    std::cout << "c = 0.5:\n";
    std::vector<double> experiment_pm_dot_0_5;
    std::vector<double> experiment_pfa_dot_0_5;
    for (size_t level = 0; level < pfa_theory_arr.size(); level++) {
        const double a = 1.8;
        const size_t N = 1000;
        const size_t iter_num = 100000;
        size_t pm_cnt = 0;
        size_t pfa_cnt = 0;

        for (size_t test_cnt = 0; test_cnt < iter_num; test_cnt++) {
            std::vector<double> X_c_0_5 { GGD10(N) };

            // 初始化 MonteCarlo 对象
            MC::MonteCarlo c_0_5(X_c_0_5);
            c_0_5.imbed(a);
            c_0_5.genLinearCorrelator();

            // 根据实验值进行检验
            pm_cnt += c_0_5.isMiss(psi_c_0_5[level]);
            pfa_cnt += c_0_5.isFalseAlarm(psi_c_0_5[level]);
        }
        experiment_pm_dot_0_5.push_back(pm_cnt / static_cast<double>(iter_num));
        experiment_pfa_dot_0_5.push_back(pfa_cnt / static_cast<double>(iter_num));
    }

    // 无攻击情况，c = 2.0
    // 嵌入强度 a = 1.5
    std::cout << "c = 2.0:\n";
    std::vector<double> experiment_pm_dot_2_0_new;
    std::vector<double> experiment_pfa_dot_2_0_new;
    // 7 个太慢了，现在只是 6 个
    for (size_t level = 0; level < pfa_theory_arr.size(); level++) {
        const double a = 1.5;
        const size_t N = 1000;
        const size_t iter_num = 100000;
        size_t pm_cnt = 0;
        size_t pfa_cnt = 0;

        for (size_t test_cnt = 0; test_cnt < iter_num; test_cnt++) {
            std::vector<double> X_c_2_0 { GGD20(N) };

            // 初始化 MonteCarlo 对象
            MC::MonteCarlo c_2_0(X_c_2_0);
            c_2_0.imbed(a);
            c_2_0.genLinearCorrelator();

            // 根据实验值进行检验
            pm_cnt += c_2_0.isMiss(psi_c_2_0[level]);
            pfa_cnt += c_2_0.isFalseAlarm(psi_c_2_0[level]);
        }
        experiment_pm_dot_2_0_new.push_back(pm_cnt / static_cast<double>(iter_num));
        experiment_pfa_dot_2_0_new.push_back(pfa_cnt / static_cast<double>(iter_num));
    }
}

void lab_2_2_2() {
    const double STD = 10;
    const size_t N = 1000;
    const double a = 1.8;

    // 理论误检率
    const std::vector<double> pfa_theory_arr {
        1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1
    };

    // 通过理论误检率计算理论阈值
    std::vector<double> psi_arr;
    for (const double& pfa_theory : pfa_theory_arr)
        psi_arr.push_back(MC::calcuPsiTheory(STD, pfa_theory, N));

    // 通过理论阈值求漏检率
    std::vector<double> pm_theory_arr;
    for (const double& pfa_theory : pfa_theory_arr)
        pm_theory_arr.push_back(MC::calcuPmTheory(pfa_theory, N, a, STD));

    std::cout << "STD_voice = 2.5:\n";
    std::vector<double> experiment_pm_dot_2_5;
    std::vector<double> experiment_pfa_dot_2_5;
    for (size_t level = 0; level < pfa_theory_arr.size(); level++) {
        const double mean = 0;
        const double STD_voice = 2.5;
        const size_t iter_num = 10000;
        size_t pm_cnt = 0;
        size_t pfa_cnt = 0;

        for (size_t index = 0; index < iter_num; index++) {
            // 生成宿主容器
            std::vector<double> GD_host { GD_vector(mean, STD, N) };

            // 初始化 MonteCarlo 对象
            MC::MonteCarlo GD_2_5(GD_host);
            GD_2_5.imbed(a);
            GD_2_5.addVoice(STD_voice);
            GD_2_5.genLinearCorrelator();

            // 根据实验值进行检验
            pm_cnt += GD_2_5.isMiss(psi_arr[level]);
            pfa_cnt += GD_2_5.isFalseAlarm(psi_arr[level]);
        }
        experiment_pm_dot_2_5.push_back(pm_cnt / static_cast<double>(iter_num));
        experiment_pfa_dot_2_5.push_back(pfa_cnt / static_cast<double>(iter_num));
    }

    std::cout << "STD_voice = 5.0:\n";
    std::vector<double> experiment_pm_dot_5_0;
    std::vector<double> experiment_pfa_dot_5_0;
    for (size_t level = 0; level < pfa_theory_arr.size(); level++) {
        const double mean = 0;
        const double STD_voice = 5.0;
        const size_t iter_num = 10000;
        size_t pm_cnt = 0;
        size_t pfa_cnt = 0;

        for (size_t index = 0; index < iter_num; index++) {
            // 生成宿主容器
            std::vector<double> GD_host { GD_vector(mean, STD, N) };

            // 初始化 MonteCarlo 对象
            MC::MonteCarlo GD_5_0(GD_host);
            GD_5_0.imbed(a);
            GD_5_0.addVoice(STD_voice);
            GD_5_0.genLinearCorrelator();

            // 根据实验值进行检验
            pm_cnt += GD_5_0.isMiss(psi_arr[level]);
            pfa_cnt += GD_5_0.isFalseAlarm(psi_arr[level]);
        }
        experiment_pm_dot_5_0.push_back(pm_cnt / static_cast<double>(iter_num));
        experiment_pfa_dot_5_0.push_back(pfa_cnt / static_cast<double>(iter_num));
    }
}

int main() {
    //lab_2_2_1();
    //lab_2_2_2();
}