#ifndef MONTECARLO_HPP
#define MONTECARLO_HPP

#define MC_NP_BEG namespace MC {
#define MC_NP_END }

#include "random_gen.hpp"
#include "iefrc.hpp"
#include <vector>
#include <algorithm> // for std::for_each
#include <array>

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
     * @brief 嵌入水印的函数
     * @param a: 嵌入强度
     */
    void imbed(const double a);

    /**
     * @brief 生成线性相关器 L(S), L(S|H1), L(S|H0)
     */
    void genLinearCorrelator();

    /**
     * @brief 计算是否漏检
     *          pm = P(detector says H0 | H1)
     *             = P(1/N \sum_{i=1}^N X_i w_i + a < \psi)
     * @param ψ: double，决策阈值
     */
    constexpr bool isMiss(const double psi) const;

    /**
     * @brief 计算是否误检
     *          pfa = P(detector says H1 | H0)
     *              = P(1/N \sum_{i=1}^N X_i w_i >= \psi)
     * @param ψ: double，决策阈值
     */
    constexpr bool isFalseAlarm(const double psi) const;

    /**
     * @brief 计算漏检概率 slides P28
     */
    inline double calcuPm(const double psi, const double a) {
        double x = sqrt(this->N)*(psi - a) / MC::variance(this->X);
        return 1 - Q(x);
    }

    /**
     * @brief 计算误检概率 slides P30
     */
    inline double calcuPfa(const double psi, const double a) {
        double x = sqrt(this->N)*psi / MC::variance(this->X);
        return Q(x);
    }
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
 * @brief Q(x) = \int_x^{+\inf} \frac{1}{\sqrt{2\pi}} exp(-t^2 / 2) dt
 * @param x: double，输入
 * @return Q(x)
 */
inline double Q(const double x) {
    return erfc(x) / 2;
}

/**
 * @brief iQ(x) = Q^{-1}(x)
 * @param x: double，输入
 * @return iQ(x)
 */
inline double iQ(const double x) {
/**
 *    Q(x) = erfc(x) / 2, efrc(iefrc(x)) = x
 * => 2Q(iefrc(x)) = x
 * => Q^{-1}(x) = 2*iefrc(x)
 */
    return ierfc(x) * 2;
}

MC_NP_END

#endif