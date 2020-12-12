#ifndef RANDOM_GEN_HPP
#define RANDOM_GEN_HPP

#define _USE_MATH_DEFINES   // to use M_PI
#include <cmath>
#include <random>       // class mt19937_64
                        // class gamma_distribution
#include <vector>
#include <ctime>      // for time

size_t BnlDstrb_single_val();
std::vector<double> GGD20(double size);
std::vector<double> GGD10(const double size);
std::vector<double> GGD05(const double size);
std::vector<double> GD_vector(double mean, double sigma, size_t size);

#endif
