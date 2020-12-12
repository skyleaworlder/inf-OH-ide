#include "montecarlo.hpp"

const std::array<int, 2> MC::MonteCarlo::w { 1, -1 };

MC::MonteCarlo::MonteCarlo(std::vector<double> X_inp)
    : X(X_inp), N(X_inp.size()) {}

void MC::MonteCarlo::imbed(const double a) {
    for (size_t i = 0; i < this->N; i++) {
        this->S_H0.push_back(X[i]);
        this->S_H1.push_back(X[i] + a*w[i % 2]);
    }
}

void MC::MonteCarlo::addVoice(const double STD) {
    std::vector<double> voice_H0 { GD_vector(0.0, STD, this->N) };
    std::vector<double> voice_H1 { GD_vector(0.0, STD, this->N) };

    for (size_t index = 0; index < this->N; index++) {
        this->S_H0[index] += voice_H0[index];
        this->S_H1[index] += voice_H1[index];
    }
}

void MC::MonteCarlo::genLinearCorrelator() {
    double sum_Si_wi_H0 = 0;
    double sum_Si_wi_H1 = 0;

    for (size_t i = 0; i < this->N; i++) {
        sum_Si_wi_H0 += this->S_H0[i]*w[i%2];
        sum_Si_wi_H1 += this->S_H1[i]*w[i%2];
    }

    this->L_S_H0 = sum_Si_wi_H0 / this->N;
    this->L_S_H1 = sum_Si_wi_H1 / this->N;
}

double MC::MonteCarlo::calcuPm(const double psi, const double a) {
    double x = sqrt(this->N)*(psi - a) / sqrt(MC::variance(this->X));
    return 1 - MC::Q(x);
}

double MC::MonteCarlo::calcuPfa(const double psi, const double a) {
    double x = sqrt(this->N)*psi / sqrt(MC::variance(this->X));
    return MC::Q(x);
}

double MC::mean(const std::vector<double> X) {
    double sum_x = 0;
    std::for_each(X.begin(), X.end(), [&sum_x](double x) {
        sum_x += x;
    });
    return sum_x / X.size();
}

double MC::variance(const std::vector<double> X) {
    double x_square = 0;
    double x_mean = MC::mean(X);

    std::for_each(X.begin(), X.end(), [&x_square, x_mean](double x) {
        x_square += pow(x-x_mean, 2);
    });

    return x_square / X.size();
}

double MC::calcuPmTheory(
    const double pfa,
    const size_t N,
    const double a,
    const double sigma_X
) {
    return 1-MC::Q(MC::iQ(pfa) - sqrt(N)*a/sigma_X);
}

double MC::calcuPsiTheory(
    const double sigma_X,
    const double pfa,
    const size_t N
) {
    return sigma_X * MC::iQ(pfa) / sqrt(N);
}