#ifndef BUTTERWORTH_CORE_HPP
#define BUTTERWORTH_CORE_HPP

#include <consteig/consteig.hpp>

namespace butterworth
{

struct Constants
{
    double fs;
    double fc;
    consteig::Matrix<double, 2, 2> A_c;
    consteig::Complex<double> p1_c;
    consteig::Complex<double> p2_c;
    consteig::Complex<double> p1_d;
    consteig::Complex<double> p2_d;
    double a1;
    double a2;
    double K;
};

constexpr Constants design()
{
    double fs = 1000.0;
    double fc = 100.0;
    double T = 1.0 / fs;
    double wc = 2.0 * PI_CONST * fc;

    double sqrt2_wc = consteig::sqrt(2.0) * wc;
    double wc_sq = wc * wc;

    consteig::Matrix<double, 2, 2> A_c{{{0.0, 1.0}, {-wc_sq, -sqrt2_wc}}};

    auto poles_c = consteig::eigenvalues(A_c);
    auto p1_c = poles_c(0, 0);
    auto p2_c = poles_c(1, 0);

    // Map each continuous-time pole to discrete-time via z = exp(s*T).
    // This is the Matched Z-Transform: poles are mapped individually rather
    // than by discretizing the full state-space system (ZOH). For this
    // filter, which has no finite zeros, both methods produce the same
    // discrete poles. The gain K is set separately to enforce DC unity gain.
    auto p1_d = consteig::exp(p1_c * T);
    auto p2_d = consteig::exp(p2_c * T);

    double a1 = -(p1_d + p2_d).real;
    double a2 = (p1_d * p2_d).real;
    double K = 1.0 + a1 + a2;

    return {fs, fc, A_c, p1_c, p2_c, p1_d, p2_d, a1, a2, K};
}

} // namespace butterworth

#endif
