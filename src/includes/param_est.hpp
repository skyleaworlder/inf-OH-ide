#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

double Gamma_inverse_solve(double target, double a, double b, double stride);
std::pair<double, double> mle_GD(std::vector<double> arr);
double mle_EXPD(std::vector<double> arr);
std::pair<double, double> moe_GD(std::vector<double> arr);
double moe_GGD(std::vector<double> arr);