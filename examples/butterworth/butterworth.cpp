#include <iomanip>
#include <iostream>

#include "butterworth_core.hpp"

// Reference the constants defined in the other object file
extern const butterworth::Constants g_butterworth_filter;

int main()
{
    const auto &filter = g_butterworth_filter;

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Designing 2nd Order Butterworth Lowpass Filter" << std::endl;
    std::cout << "Cutoff: " << filter.fc << " Hz, Sampling Rate: " << filter.fs
              << " Hz" << std::endl;
    std::cout << "Continuous System Matrix A:" << std::endl;
    std::cout << "[ " << filter.A_c(0, 0) << ", " << filter.A_c(0, 1) << " ]"
              << std::endl;
    std::cout << "[ " << filter.A_c(1, 0) << ", " << filter.A_c(1, 1) << " ]"
              << std::endl;

    std::cout << "\nContinuous-time Poles (Eigenvalues of A):" << std::endl;
    std::cout << "p1 = " << filter.p1_c.real << " + j" << filter.p1_c.imag
              << std::endl;
    std::cout << "p2 = " << filter.p2_c.real << " + j" << filter.p2_c.imag
              << std::endl;

    std::cout << "\nDiscrete-time Poles (Mapped via z = exp(sT)):" << std::endl;
    std::cout << "z1 = " << filter.p1_d.real << " + j" << filter.p1_d.imag
              << std::endl;
    std::cout << "z2 = " << filter.p2_d.real << " + j" << filter.p2_d.imag
              << std::endl;

    std::cout << "\nFilter Coefficients (Denominator):" << std::endl;
    std::cout << "a1 = " << filter.a1 << std::endl;
    std::cout << "a2 = " << filter.a2 << std::endl;

    std::cout << "Gain K = " << filter.K << std::endl;

    std::cout << "\nFinal Digital Filter Difference Equation:" << std::endl;
    std::cout << "y[n] = " << filter.K << " * x[n] - (" << filter.a1
              << ") * y[n-1] - (" << filter.a2 << ") * y[n-2]" << std::endl;

    return 0;
}
