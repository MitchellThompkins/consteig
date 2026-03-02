#include "../consteig.hpp"
#include "example_helper.hpp"
#include <iostream>

/**
 * Performance Requirements for DC Motor Control
 *
 * Defines the acceptable operational envelope for the motor's dynamics.
 */
struct PerformanceRequirements
{
    double min_convergence; // Most positive real part allowed (rad/s)
    double max_convergence; // Most negative real part allowed (rad/s)
    double min_damping;     // Minimum damping ratio (zeta)
};

/**
 * Helper to check stability and performance at compile-time
 */
template <typename T, consteig::Size S>
constexpr bool check_performance(
    const consteig::Matrix<consteig::Complex<T>, S, 1> &eigs,
    PerformanceRequirements reqs)
{
    for (consteig::Size i = 0; i < S; ++i)
    {
        T sigma = eigs(i, 0).real;
        T omega = eigs(i, 0).imag;

        // Stability & Convergence Rate (Real part check)
        if (sigma >= reqs.min_convergence || sigma <= reqs.max_convergence)
        {
            return false;
        }

        // Damping Ratio Analysis (for complex pairs)
        if (consteig::abs(omega) > 1e-6)
        {
            T zeta = -sigma / consteig::sqrt(sigma * sigma + omega * omega);
            if (zeta < reqs.min_damping)
            {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    // --- Motor Parameters (Defined at compile-time) ---
    static constexpr double J = 3.2284e-6;   // Moment of inertia [kg⋅m²]
    static constexpr double b = 3.5077e-6;   // Viscous friction [N⋅m⋅s/rad]
    static constexpr double K_m = 0.0274;    // Motor constant [V⋅s/rad]
    static constexpr double R = 4.0;         // Armature resistance [Ω]
    static constexpr double L = 2.75e-6;     // Armature inductance [H]

    static constexpr consteig::Size s{4};

    // --- State Space Assembly (constexpr) ---
    // Augmented state: x = [position; velocity; current; integral_error]
    static constexpr consteig::Matrix<double, s, s> A{
        {{{0.0, 1.0, 0.0, 0.0},
          {0.0, -b / J, K_m / J, 0.0},
          {0.0, -K_m / L, -R / L, 0.0},
          {-1.0, 0.0, 0.0, 0.0}}}};

    static constexpr consteig::Matrix<double, s, 1> B{
        {{{0.0}, {0.0}, {1.0 / L}, {0.0}}}};

    // --- Control Constraints ---
    static constexpr PerformanceRequirements limits = {
        -80.0,  // min_convergence: slowest pole allowed (settling time < 40ms)
        -1e9,   // max_convergence: very fast poles allowed
        0.504   // min_damping: avoid overshoot > 16%
    };

    // =========================================================================
    // SCENARIO 1: Good Tuning (With Integral Action)
    // =========================================================================
    static constexpr double K1_good = 0.021871;
    static constexpr double K2_good = -0.027279;
    static constexpr double K3_good = -3.997253;
    static constexpr double K4_good = -1.749699;

    static constexpr consteig::Matrix<double, 1, s> K_good{
        {{{K1_good, K2_good, K3_good, K4_good}}}};

    static constexpr consteig::Matrix<double, s, s> A_cl_good{A - B * K_good};
    static constexpr auto eigs_good = consteig::eigvals(A_cl_good);

    // Are they acceptable? (This compiles cleanly!)
    static_assert(check_performance(eigs_good, limits),
                  "Gains fail performance requirements - retune!");

    std::cout << "System Parameters (constexpr): J=" << J << ", b=" << b
              << ", K_m=" << K_m << "\n\n";
    std::cout << "--- SCENARIO 1: Good Tuning (With Integral Action) ---\n";
    std::cout << "Gains [K=" << K1_good << ", " << K2_good << ", "
              << K3_good << ", " << K4_good << "] passed all checks.\n";
    std::cout << "Resulting Poles (behavior):\n";
    for (consteig::Size i = 0; i < s; ++i)
    {
        std::cout << "  " << eigs_good(i, 0).real << " + "
                  << eigs_good(i, 0).imag << "i\n";
    }

    // =========================================================================
    // SCENARIO 2: PID Tuning (Kp=21, Kd=0.05, Ki=200)
    // =========================================================================
    static constexpr double K1_bad = 21.0;
    static constexpr double K2_bad = 0.05;
    static constexpr double K3_bad = 0.0;
    static constexpr double K4_bad = -200.0;

    static constexpr consteig::Matrix<double, 1, s> K_bad{
        {{{K1_bad, K2_bad, K3_bad, K4_bad}}}};

    static constexpr consteig::Matrix<double, s, s> A_cl_bad{A - B * K_bad};
    static constexpr auto eigs_bad = consteig::eigvals(A_cl_bad);

    // This assertion WILL fail the build! 
    static_assert(check_performance(eigs_bad, limits), "SYSTEM REJECTED: Performance requirements not met!");

    std::cout << "\n--- SCENARIO 2: PID Tuning (Kp=21, Kd=0.05, Ki=200) ---\n";
    std::cout << "Gains [K=" << K1_bad << ", " << K2_bad << ", "
              << K3_bad << ", " << K4_bad << "] rejection status checked.\n";
    std::cout << "Resulting Poles:\n";
    for (consteig::Size i = 0; i < s; ++i)
    {
        double sigma = eigs_bad(i, 0).real;
        double omega = eigs_bad(i, 0).imag;
        std::cout << "  " << sigma << " + " << omega << "i";
        if (consteig::abs(omega) > 1e-6)
        {
            std::cout << " (zeta = "
                      << -sigma / consteig::sqrt(sigma * sigma + omega * omega)
                      << ")";
        }
        std::cout << "\n";
    }

    return 0;
}
