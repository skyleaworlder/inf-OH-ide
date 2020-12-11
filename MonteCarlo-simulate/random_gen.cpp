#include "random_gen.hpp"

std::uniform_real_distribution<double> U_1 {0,1};
std::uniform_real_distribution<double> U_2 {0,1};
std::uniform_int_distribution<int> B {0,1};
constexpr double twi_pi = M_PI * 2;
std::random_device rd;
std::default_random_engine rng {rd()};

/* Bernoulli Distribution
    return 0(P = 0.5 theoretically) or 1(P = 0.5 theoretically)
*/
size_t BnlDstrb_single_val() {
    return B(rng);
}

/* GGD with shape param c and scope param beta
    E = Gamma(1/c, 1/(beta^c))^(1/c)  (X = 1)
        -Gamma(1/c, 1/(beta^c))^(1/c) (X = 0)
    according to slide fundamentals-cn-ppt--P34
*/
std::vector<double> GGD_vector(
    double beta,
    double c,
    double size
) {
    // get one val Gamma distribution
    std::gamma_distribution<double> G {1/c, 1/pow(beta, c)};
    std::vector<double> arr;
    for (size_t index = 0; index < size; ++index) {
        size_t Bnl_res = BnlDstrb_single_val();
        double E = pow(G(rng),1/c);
        if (Bnl_res == 1)
            arr.push_back(E);
        else
            arr.push_back(-E);
    }
    return arr;
}