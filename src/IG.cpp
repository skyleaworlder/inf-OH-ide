#include "IG.hpp"

/*
    From 《Numerical Recipes in C》
    it's supposed to read from chapter 6.1

    And implementation(gammln, gcf, gser...)
    are given in section 6.1 & 6.2
*/
double gammln(const double input) {
    double x, y, tmp, ser;
    const double cof[6] = {
        76.18009172947146,
        -86.50532032941677,
        24.01409824083091,
        -1.231739572450155,
        0.1208650973866179e-2,
        -0.5395239384953e-5
    };
    y = x = input;
    tmp = x + 5.5;
    tmp -= (x + 0.5) * log(tmp);
    ser = 1.000000000190015;
    for (size_t j = 0; j < 6; ++j)
        ser += cof[j] / ++y;
    return -tmp + log(2.5066282746310005 * ser / x);
}

/* get Q(alpha, x), which equals 1 - P(alpha, x)
    return Q(alpha, x) and ln\Gamma(alpha).
    using series.

    first is Q(alpha, x), while second is ln\Gamma(alpha)
*/
std::pair<double, double> gser(
    const double alpha,
    const double x
) {
    const size_t ITMAX = 100;
    const double EPS = std::numeric_limits<double>::epsilon();
    double sum, del;
    double ap_variable = alpha;

    double gamser;
    double gln = gammln(alpha);
    if (x <= 0) {
        if (x < 0) {
            std::cout << "x must be greater than 0" << std::endl;
            exit(EXIT_FAILURE);
        }
        gamser = 0;
        return std::pair<double, double> {gamser, gln};
    }
    else {
        del = sum = 1.0 / ap_variable;
        for (size_t n = 0; n < ITMAX; ++n) {
            ++ap_variable;
            del *= x / ap_variable;
            sum += del;
            if (fabs(del) < fabs(sum) * EPS) {
                gamser = sum * exp(-x + alpha*log(x) - gln);
                return std::pair<double, double> {gamser, gln};
            }
        }
    }
    std::cout << "alpha is too large, ITMAX is too small." << std::endl;
    exit(EXIT_FAILURE);
}

/* incompatible Gamma function
    Q(alpha, x)
        using continuant
*/
std::pair<double, double> gcf(
    const double alpha,
    const double x
) {
    const size_t ITMAX = 100;
    const double EPS = std::numeric_limits<double>::epsilon();
    const double FPMIN = std::numeric_limits<double>::min() / EPS;

    double gln = gammln(alpha);
    double b = x + 1.0 - alpha;
    double c = 1.0 / FPMIN;
    double d = 1.0 / b;
    double h = d;
    double an, del;
    size_t index;
    for (index = 0; index <= ITMAX; ++index) {
        an = -index*(index - alpha);
        b += 2.0;
        d = an * d + b;
        if (fabs(d) < FPMIN)
            d = FPMIN;
        c = b + an / c;
        if (fabs(c) < FPMIN)
            c = FPMIN;
        d = 1.0 / d;
        del = d * c;
        h *= del;
        if (fabs(del - 1.0) <= EPS)
            break;
    }
    if (index > ITMAX) {
        std::cout << "alpha too large, and ITMAX too small in gcf." << std::endl;
        exit(EXIT_FAILURE);
    }
    double gammcf = exp(-x + alpha*log(x) - gln) * h;
    return std::pair<double, double> { gammcf, gln };
}

/* incompatible gamma function
    F_{\Gamma(alpha)}(x) =
        1 / (\Gamma(\alpha)) * \int_0^{x} exp(-t)*t^{\alpha-1}
*/
double gammp(
    const double alpha,
    const double x
) {
    if (x < 0 || alpha <= 0) {
        std::cout << "alpha less than 0, and x less than 0, error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (x < alpha + 1.0) {
        std::pair<double, double> res {gser(alpha, x)};
        return res.first;
    }
    else {
        std::pair<double, double> res {gcf(alpha, x)};
        return 1.0 - res.first;
    }
}

/*  Q(alpha, x) = 1 - F(alpha, x)
*/
double gammq(
    const double alpha,
    const double x
) {
    return 1 - gammp(alpha, x);
}