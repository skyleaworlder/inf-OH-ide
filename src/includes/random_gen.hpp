#define _USE_MATH_DEFINES   // to use M_PI
#include <iostream>
#include <cmath>
#include <random>       // class uniform_real_distribution
                        // class uniform_int_distribution
                        // class gamma_distribution
#include <limits>
#include <vector>
#include <cassert>      // assert function
#include <string>
#include "IG.hpp"

double GuassDstrb_single_val(double miu, double sigma);
double ExpDstrb_single_val(double beta);
size_t BnlDstrb_single_val();
std::vector<double> GGD_vector(double beta, double c, double size);
std::pair<
    std::vector<double>,
    std::vector<double>
> GGD_theoretical_CDF(
    double beta, double c, double size,
    double min, double max
);