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
        // 'sigma' (the real part) dictates the exponential decay envelope of
        // the response e^(sigma*t). It must be strictly negative for stability.
        // We also constrain it between min and max bounds to ensure the system
        // is neither too sluggish nor demands too much instantaneous control
        // effort.
        if (sigma >= reqs.min_convergence || sigma <= reqs.max_convergence)
        {
            return false;
        }

        // Damping Ratio Analysis (for complex pairs)
        if (consteig::abs(omega) > 1e-6)
        {
            // For a complex pole pair p = sigma +/- j*omega, the natural
            // frequency is wn = sqrt(sigma^2 + omega^2). The damping ratio
            // (zeta) is defined by the relation: sigma = -zeta * wn.
            // Rearranging gives: zeta = -sigma / wn = -sigma / sqrt(sigma^2 +
            // omega^2). A higher zeta means less overshoot and ringing. We
            // enforce a minimum damping ratio so the physical system (e.g., a
            // gearbox) isn't subjected to excessive mechanical stress.
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
    static constexpr double J = 0.01;   // Moment of inertia [kg⋅m²]
    static constexpr double b = 0.1;    // Viscous friction [N⋅m⋅s/rad]
    static constexpr double K_m = 0.01; // Motor constant [V⋅s/rad]
    static constexpr double R = 1.0;    // Armature resistance [Ω]
    static constexpr double L = 0.5;    // Armature inductance [H]

    static constexpr consteig::Size s{3};

    // --- State Space Assembly (constexpr) ---
    // x = [position; velocity; current]
    static constexpr consteig::Matrix<double, s, s> A{
        {{{0.0, 1.0, 0.0}, {0.0, -b / J, K_m / J}, {0.0, -K_m / L, -R / L}}}};

    static constexpr consteig::Matrix<double, s, 1> B{
        {{{0.0}, {0.0}, {1.0 / L}}}};

    // --- Control Constraints ---
    static constexpr PerformanceRequirements limits = {
        -2.0,  // min_convergence: slowest pole allowed
        -50.0, // max_convergence: fastest pole allowed (limit current)
        0.55   // min_damping: avoid gearbox stress
    };

    // =========================================================================
    // SCENARIO 1: A Firmware Engineer tunes a good set of PID gains.
    // =========================================================================

    // Gains tuned by hand / Ziegler-Nichols / etc.
    static constexpr double Kp_good = 123.0;
    static constexpr double Kd_good = 20.49;
    static constexpr double Ki_eff_good = 2.0;

    static constexpr consteig::Matrix<double, 1, 3> K_good{
        {{{Kp_good, Kd_good, Ki_eff_good}}}};

    // Where did my poles end up? The eigenvalue solver is the bridge between
    // "gains I chose" and "behaviour I get."
    static constexpr consteig::Matrix<double, s, s> A_cl_good{A - B * K_good};
    static constexpr auto eigs_good = consteig::eigvals(A_cl_good);

    // Are they acceptable? (This compiles cleanly!)
    static_assert(check_performance(eigs_good, limits),
                  "Gains fail performance requirements - retune!");

    std::cout << "System Parameters (constexpr): J=" << J << ", b=" << b
              << ", K_m=" << K_m << "\n\n";
    std::cout << "--- SCENARIO 1: Good PID Tuning ---\n";
    std::cout << "Gains [Kp=" << Kp_good << ", Kd=" << Kd_good
              << ", Ki_eff=" << Ki_eff_good << "] passed all checks.\n";
    std::cout << "Resulting Poles (behavior):\n";
    for (consteig::Size i = 0; i < s; ++i)
    {
        std::cout << "  " << eigs_good(i, 0).real << " + "
                  << eigs_good(i, 0).imag << "i\n";
    }

    // =========================================================================
    // SCENARIO 2: An overly aggressive tune that causes dangerous oscillation.
    // =========================================================================

    // Aggressively high proportional and derivative gains
    static constexpr double Kp_bad = 375.0;
    static constexpr double Kd_bad = 62.49;
    static constexpr double Ki_eff_bad = 2.0;

    static constexpr consteig::Matrix<double, 1, 3> K_bad{
        {{{Kp_bad, Kd_bad, Ki_eff_bad}}}};

    static constexpr consteig::Matrix<double, s, s> A_cl_bad{A - B * K_bad};
    static constexpr auto eigs_bad = consteig::eigvals(A_cl_bad);

    // This assertion will intentionally fail the build!
    // The compiler prevents the firmware with bad dynamics from ever executing.
    // comment the below static assert out to actually build
    static constexpr bool bad_perf_ok = check_performance(eigs_bad, limits);
    static_assert(bad_perf_ok, "SYSTEM REJECTED: Underdamped system detected "
                               "(damping ratio too low) - retune!");

    std::cout << "\n--- SCENARIO 2: Aggressive PID Tuning ---\n";
    std::cout << "Gains [Kp=" << Kp_bad << ", Kd=" << Kd_bad
              << ", Ki_eff=" << Ki_eff_bad << "] rejected.\n";
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
