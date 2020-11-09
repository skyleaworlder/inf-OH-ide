#include "random_gen.hpp"

std::uniform_real_distribution<double> U_1 {0,1};
std::uniform_real_distribution<double> U_2 {0,1};
std::uniform_int_distribution<int> B {0,1};
constexpr double twi_pi = M_PI * 2;
std::random_device rd;
std::default_random_engine rng {rd()};

/* Box-Muller method
    http://www.lmpt.univ-tours.fr/~nicolis/Licence_NEW/08-09/boxmuller.pdf

    it has to be noticed that ---
        u_2 is required!
        for s, t are different random num in "z_0 = sqrt(-2log(s))*cos(2*pi*t)"
*/
double GuassDstrb_single_val(
    double miu,
    double sigma
) {
    double u_1 = U_1(rng), u_2 = U_2(rng);
    assert(u_2 <= 1 && u_2 > 0);
    double R = sqrt(-2.0 * log(u_2));
    double z_0 = R * cos(twi_pi * u_1);
    return miu + sigma * z_0;
}

/* Reverse-Function method
    slides: fundamentals-cn-ppt--P46
*/
double ExpDstrb_single_val(
    double beta
) {
    double u = U_1(rng);
    return (-beta) * log(1-u);
}

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

/* GGD theoretical value
    return: x_arr and y_arr (CDF value)

    beta is scale param
    c is shape param
    size is x_arr or y_arr's size
    min is bound of GGD sample gain fron GGD_vector.
    so does max.
*/
std::pair<
    std::vector<double>,
    std::vector<double>
> GGD_theoretical_CDF(
    double beta, double c, double size,
    double min, double max
) {
    std::vector<double> x_arr, y_arr;
    const double stride = (max - min) / size;
    for (size_t index = 0; index <= size; ++index)
        x_arr.push_back(min + stride * index);

    for (double x : x_arr) {
        double P_res;
        if (x < 0) {
            P_res = gammp(1.0/c, pow(-x, c)*pow(beta, c));
            y_arr.push_back(0.5 * (1-P_res));
        }
        else {
            P_res = gammp(1.0/c, pow(x, c)*pow(beta, c));
            y_arr.push_back(0.5*P_res + 0.5);
        }
    }

    return std::pair<
        std::vector<double>,
        std::vector<double>
    > {x_arr, y_arr};
}