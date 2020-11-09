#include "param_est.hpp"

/* Gamma inverse solve
    target is the value of the given expression
    need [a,b] and stride
    and c \in [a,b]
*/
double Gamma_inverse_solve(
    double target,
    double a, double b,
    double stride=0.01
) {
    assert(a+stride <= b);
    double c = a;           // c \in [a, b], init by a
    double delta = target;  // target must ge 0, if a,b > 0

    size_t cnt = 0;
    double c_i = a;    // c_i is probe
    while (c_i <= b) {
        // target = expr(c)
        // tmp_delta = |expr(c) - expr(c_i)|
        // c_i = a + cnt*stride
        double expr = tgamma(2/c_i) / sqrt(tgamma(1/c_i) * tgamma(3/c_i));
        double tmp_delta = fabs(target - expr);
        if (delta > tmp_delta) {
            delta = tmp_delta;
            c = a + cnt*stride;
        }
        ++cnt;
        c_i = a + cnt*stride;
    }
    return c;
}

/* Maximum Likelihood Estimation(MLE)
    https://zh.m.wikipedia.org/wiki/%E6%9C%80%E5%A4%A7%E4%BC%BC%E7%84%B6%E4%BC%B0%E8%AE%A1
*/
std::pair<double, double> mle_GD(
    std::vector<double> arr
) {
    double miu, sigma;
    double sum_x_i = 0;
    for (double x_i : arr)
        sum_x_i += x_i;
    miu = sum_x_i / arr.size();

    double square_sum_x_i_minus_miu = 0;
    for (double x_i : arr)
        square_sum_x_i_minus_miu += pow(x_i - miu, 2);
    sigma = sqrt(square_sum_x_i_minus_miu / arr.size());

    return std::pair<double, double>(miu, sigma);
}

/* Maximum Likelihood Estimation(MLE of Exp_distribution)
    \beta = \dfrac{\sum_{i=1}^{n} x_i}{n}
    (ps: no reference, self-derivation, more detail in lab-report)
*/
double mle_EXPD(
    std::vector<double> arr
) {
    double sum_x_i = 0;
    for (double x_i : arr)
        sum_x_i += x_i;
    return sum_x_i / arr.size();
}

/* Moment Estimation of GD (ME)
    miu = E(X) = sum(X_i) / N
    miu^2 + sigma^2 = E(X^2) = sum(X_i^2) / N
*/
std::pair<double, double> moe_GD(
    std::vector<double> arr
) {
    double miu, sigma;
    double sum_x_i = 0, square_sum_x_i = 0;
    for (double x_i : arr) {
        sum_x_i += x_i;
        square_sum_x_i += pow(x_i, 2);
    }
    miu = sum_x_i / arr.size();
    double miu_and_sigma_square = square_sum_x_i / arr.size();
    sigma = sqrt(miu_and_sigma_square - pow(miu, 2));

    return std::pair<double, double>(miu, sigma);
}

/* Moment Estimation of GGD (ME)
    return sigma
*/
double moe_GGD(
    std::vector<double> arr
) {
    double sum_square_x_i = 0, sum_abs_x_i = 0;
    for (double elem : arr) {
        sum_square_x_i += pow(elem, 2);
        sum_abs_x_i += fabs(elem);
    }
    double sigma = sqrt(sum_square_x_i / arr.size());
    double E_abs_x = sum_abs_x_i / arr.size();

    bool flag = false;
    double c;
    do {
        double a, b, stride;
        std::cout << "note: you must figure out c's scope with [a, b]:"
                <<  "      (double, recommand int)" << std::endl;
        std::cin >> a >> b;
        std::cout << "      you must figure out stride:"
                << "(double, recommand [.001, 1], default .01)" << std::endl;
        std::cin >> stride;
        c = Gamma_inverse_solve(
            E_abs_x / sigma,
            a, b, stride
        );
        flag = (c == a || c == b);
    } while(flag);
    return c;
}
