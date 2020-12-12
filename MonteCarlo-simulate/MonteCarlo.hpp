#ifndef MONTECARLO_HPP
#define MONTECARLO_HPP

#define MC_NP_BEG namespace MC {
#define MC_NP_END }

#include "random_gen.hpp"
#include "iefrc.hpp"
#include <vector>
#include <algorithm> // for std::for_each
#include <array>
#include <cassert>
#include <iostream>

MC_NP_BEG

class MonteCarlo {
public:
    static const std::array<int, 2> w;

    std::vector<double> X;
    std::vector<double> S_H0;
    std::vector<double> S_H1;

    size_t N;

    double L_S_H0;
    double L_S_H1;

public:

    MonteCarlo(std::vector<double> X_inp);
    ~MonteCarlo() {};

    /**
     * @brief 嵌入水印的函数，
     *          通过这个函数修改 a
     *          也是通过这个函数初始化 S_H0, S_H1
     * @param a: 嵌入强度
     */
    void imbed(const double a);

    /**
     * @brief 对 S_H0 和 S_H1 施加噪声
     *          需要注意的是，这是对 S_H0 / S_H1 的直接修改
     *          接下来如果继续添加噪声，噪声会叠加
     * @param STD: double，标准差
     */
    void addVoice(const double STD);

    /**
     * @brief 生成线性相关器 L(S), L(S|H1), L(S|H0)
     *          由于是根据 S_H0, S_H1 来计算的
     *          与 S_H0, S_H1 相关
     *          因此不需要给嵌入强度 a
     *
     *          会产生 L_S_H0 和 L_S_H1
     */
    void genLinearCorrelator();

    /**
     * @brief 计算是否漏检
     *          根据 L_S_H1 计算
     *          pm = P(detector says H0 | H1)
     *             = P(1/N \sum_{i=1}^N X_i w_i + a < \psi)
     * @param ψ: double，决策阈值
     */
    constexpr bool isMiss(const double psi) const {
        return (this->L_S_H1 < psi);
    }

    /**
     * @brief 计算是否误检
     *          根据 L_S_H0 计算
     *          pfa = P(detector says H1 | H0)
     *              = P(1/N \sum_{i=1}^N X_i w_i >= \psi)
     * @param ψ: double，决策阈值
     */
    constexpr bool isFalseAlarm(const double psi) const {
        return (this->L_S_H0 > psi);
    }

    /**
     * @brief 计算漏检概率 slides P28
     *          根据 X 计算
     */
    double calcuPm(const double psi, const double a);

    /**
     * @brief 计算误检概率 slides P30
     */
    double calcuPfa(const double psi, const double a);
};

/**
 * @brief 计算均值
 * @param X: vector<double>，给定数组
 * @return double，均值
 */
double mean(const std::vector<double> X);

/**
 * @brief 计算方差
 * @param X: vector<double>，给定数组
 * @return double，方差
 */
double variance(const std::vector<double> X);

/**
 * @brief 计算理论漏检概率
 *      因为 p_{fa} = Q[\frac{\sqrt{N} \psi}{\sigma_X}]
 *      所以 p_m = 1-Q[\frac{\sqrt{N}(\psi-a)}{\sigma_X}]
 *           p_m = 1-Q[Q^{-1}(p_fa) - \frac{\sqrt{N}a}{\sigma_X}]
 * @param pfa: double，给出误检概率
 * @param N: size_t，样本总数
 * @param a: double，给出的嵌入强度
 * @param sigma_X: double，给出标准差
 * @return double 理论漏检概率
 */
double calcuPmTheory(
    const double pfa,
    const size_t N,
    const double a,
    const double sigma
);

/**
 * @brief 计算理论阈值 slides-实验二说明 P12
 * @param sigma_X: double，给出标准差
 * @param pfa: double，给出用于计算理论阈值的理论 pfa
 * @param N: size_t，样本总数
 */
double calcuPsiTheory(
    const double sigma_X,
    const double pfa,
    const size_t N
);

/**
 * @brief Q(x) = \int_x^{+\inf} \frac{1}{\sqrt{2\pi}} exp(-t^2 / 2) dt
 *          Q(x) = erfc(x) / 2
 * @param x: double，输入
 * @return Q(x)
 */
inline double Q(const double x) {
    //std::cout << x << '\n';
    // x \in (-\inf, +\inf), y \in (0, 1)
    return erfc(x) / 2.0;
}

/**
 * @brief iQ(x) = Q^{-1}(x)
 * @param x: double，输入
 * @return iQ(x)
 */
inline double iQ(const double x) {
    //std::cout << x << '\n';
    // x \in (0, 1), y \in (-\inf, +\inf)
    assert(x > -0.01 && x < 1.01);
/**
 *    Q(x) = erfc(x) / 2, efrc(iefrc(x)) = x
 * => 2Q(iefrc(x)) = x => Q(iefrc(x)) = x/2 => iefrc(x) = Q^{-1}(x/2)
 * => Q^{-1}(x) = iefrc(2*x)
 */
    return ierfc(x*2.0);
}

MC_NP_END

#endif