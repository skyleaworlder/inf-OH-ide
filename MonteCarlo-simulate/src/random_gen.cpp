#include "random_gen.hpp"

std::random_device rd;
std::mt19937_64 rng(rd());

std::vector<double> GGD20(
    const double size
) {
	std::normal_distribution<double> distribution(0, 10.0);
    std::vector<double> to_ret;
	for (int i = 0; i < size; ++i)
		to_ret.push_back(distribution(rng));
	return to_ret;
}

std::vector<double> GGD10(
    const double size
) {
	std::exponential_distribution<double> distribution(1.0);
	std::vector<double> to_ret;
    for (int i = 0; i < size; ++i)
		to_ret.push_back(distribution(rng));

	srand((unsigned)time(0));
	for (int i = 0; i < size; i++)
        to_ret[i] = (rand() % 2 ? to_ret[i] : -to_ret[i]);
	return to_ret;
}

std::vector<double> GGD05(
    const double size
) {
	std::vector<double> num1, num2;
	std::exponential_distribution<double> distribution(1.0);

	for (int i = 0; i < size; ++i)
        num1.push_back(distribution(rng));
	for (int i = 0; i < size; ++i)
		num2.push_back(distribution(rng));

    std::vector<double> to_ret;
	srand((int)1);
	for (int i = 0; i < size; i++) {
		double tmp = pow(num1[i] + num2[i], 2);
		to_ret.push_back(rand() % 2 ? tmp : -tmp);
	}
	return to_ret;
}

/**
 * @brief 产生高斯分布
 * @param mean: double，平均值
 * @param sigma: double，标准差
 * @param size: 产生数量
 * @return vector<double> 一个高斯分布的 vector
 */
std::vector<double> GD_vector(
    double mean,
    double sigma,
    size_t size
) {
    std::normal_distribution<double> G { mean, sigma };
    std::vector<double> to_ret;
    for (size_t cnt = 0; cnt < size; cnt++) {
        double Guass_val = G(rng);
        to_ret.push_back(Guass_val);
    }
    return to_ret;
}