#include <vector>
#include <iostream>

/* Gauss Distribution test function
    using 3-sigma rule to test Gauss Distribution
*/
static void GuassDstrb_test(
    std::vector<double> arr,
    double miu,
    double sigma
) {
    size_t cnt_sigma = 0, cnt_2sigma = 0, cnt_3sigma = 0, cnt_oth = 0;

    for (double elem : arr) {
        switch (int(fabs(elem-miu) / sigma))
        {
        case 0:
            ++cnt_sigma;
            break;
        case 1:
            ++cnt_2sigma;
            break;
        case 2:
            ++cnt_3sigma;
            break;
        default:
            ++cnt_oth;
            break;
        }
    }

    std::cout << std::endl
            << "[miu-sigma, miu+sigma]: "<< cnt_sigma << std::endl
            << "[miu-2sigma, miu-sigma] U [miu+sigma, miu+2sigma]: " << cnt_2sigma << std::endl
            << "[miu-3sigma, miu-2sigma] U [miu+2sigma, miu+3sigma]: " << cnt_3sigma << std::endl
            << "[-inf, miu-3sigma] U [miu+3sigma, inf]: " << cnt_oth << std::endl;
}

/* Bernoulli test function
*/
static void BnlDstrb_test(
    std::vector<int> arr
) {
    size_t zero_num = 0, one_num = 0;
    for (int elem : arr)
        if (elem == 0)
            ++zero_num;
        else
            ++one_num;
    std::cout << "zero: " << zero_num << std::endl
            << "one: " << one_num << std::endl;
}