#include <cmath>
#include <complex>
#include <iostream>

#include "../consteig.hpp"

// Example: Designing a digital filter using eigenvalue mapping (Matched
// Z-Transform / ZOH Method) We find the continuous-time eigenvalues (poles) of
// a Butterworth filter matrix and map them to the Z-domain to form the digital
// filter coefficients.

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    // Filter Specifications
    // 2nd Order Butterworth Lowpass Filter
    constexpr double fs = 1000.0;           // Sampling Frequency (Hz)
    constexpr double fc = 100.0;            // Cutoff Frequency (Hz)
    constexpr double T = 1.0 / fs;          // Sampling Period
    constexpr double wc = 2.0 * M_PI * fc;  // Angular Cutoff Frequency (rad/s)

    // Continuous-time State Space Matrix A for 2nd order Butterworth
    // H(s) = wc^2 / (s^2 + sqrt(2)*wc*s + wc^2)
    // Characteristic Equation: s^2 + sqrt(2)*wc*s + wc^2 = 0
    // Companion Matrix Form:
    // A = [   0          1      ]
    //     [ -wc^2   -sqrt(2)*wc ]

    constexpr double sqrt2_wc = consteig::sqrt(2.0) * wc;
    constexpr double wc_sq = wc * wc;

    // A_c(0, 0) = 0.0;
    // A_c(0, 1) = 1.0;
    // A_c(1, 0) = -wc_sq;
    // A_c(1, 1) = -sqrt2_wc;
    static constexpr consteig::Matrix<double, 2, 2> A_c{
        {{{0.0, 1.0}, {-wc_sq, -sqrt2_wc}}}};

    std::cout << "Designing 2nd Order Butterworth Lowpass Filter" << std::endl;
    std::cout << "Cutoff: " << fc << " Hz, Sampling Rate: " << fs << " Hz"
              << std::endl;
    std::cout << "Continuous System Matrix A:" << std::endl;
    std::cout << "[ " << A_c(0, 0) << ", " << A_c(0, 1) << " ]" << std::endl;
    std::cout << "[ " << A_c(1, 0) << ", " << A_c(1, 1) << " ]" << std::endl;

    // 1. Find Continuous-time Eigenvalues (Poles) using Consteig
    // This calculates the poles directly from the system matrix.
    constexpr auto poles_c = consteig::eigvals(A_c);

    std::cout << "\nContinuous-time Poles (Eigenvalues of A):" << std::endl;
    std::cout << "p1 = " << poles_c(0, 0).real << " + j" << poles_c(0, 0).imag
              << std::endl;
    std::cout << "p2 = " << poles_c(1, 0).real << " + j" << poles_c(1, 0).imag
              << std::endl;

    // 2. Map Poles to Z-Domain (Matched Z-Transform)
    // z = exp(sT)
    // We use std::complex here for the exponential function

    std::complex<double> p1_c(poles_c(0, 0).real, poles_c(0, 0).imag);
    std::complex<double> p2_c(poles_c(1, 0).real, poles_c(1, 0).imag);

    std::complex<double> p1_d = std::exp(p1_c * T);
    std::complex<double> p2_d = std::exp(p2_c * T);

    std::cout << "\nDiscrete-time Poles (Mapped via z = exp(sT)):" << std::endl;
    std::cout << "z1 = " << p1_d.real() << " + j" << p1_d.imag() << std::endl;
    std::cout << "z2 = " << p2_d.real() << " + j" << p2_d.imag() << std::endl;

    // 3. Form Digital Filter Transfer Function Denominator
    // D(z) = (z - z1)(z - z2) = z^2 - (z1 + z2)z + (z1 * z2)
    // y[n] = b0*x[n] - a1*y[n-1] - a2*y[n-2]
    // where D(z) corresponds to 1 + a1*z^-1 + a2*z^-2 in the difference
    // equation (after normalizing by z^2)

    // Coefficients for the characteristic polynomial P(z) = z^2 + coeff1*z +
    // coeff2
    double coeff1 = -(p1_d + p2_d).real();
    double coeff2 = (p1_d * p2_d).real();

    // In difference equation y[n] = ... - a1*y[n-1] - a2*y[n-2]
    // These correspond to the polynomial coefficients directly.
    double a1 = coeff1;
    double a2 = coeff2;

    std::cout << "\nFilter Coefficients (Denominator):" << std::endl;
    std::cout << "a1 = " << a1 << std::endl;
    std::cout << "a2 = " << a2 << std::endl;

    // 4. Determine Gain (Numerator) for Unity DC Gain
    // At DC (w=0, z=1), Gain = K / (1 + a1 + a2)
    // We want Gain = 1, so K = 1 + a1 + a2

    double K = 1.0 + a1 + a2;

    std::cout << "Gain K = " << K << std::endl;

    std::cout << "\nFinal Digital Filter Difference Equation:" << std::endl;
    std::cout << "y[n] = " << K << " * x[n] - (" << a1 << ") * y[n-1] - (" << a2
              << ") * y[n-2]" << std::endl;

    return 0;
}
