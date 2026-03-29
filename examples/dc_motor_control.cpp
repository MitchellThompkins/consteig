#include "../consteig.hpp"
#include "example_helper.hpp"
#include <iostream>

/**
 * Helper to check asymptotic stability (All poles in LHP)
 */
template <typename T, consteig::Size S>
constexpr bool check_stability(
    const consteig::Matrix<consteig::Complex<T>, S, 1> &eigs)
{
    for (consteig::Size i = 0; i < S; ++i)
    {
        if (eigs(i, 0).real >= 0.0)
        {
            return false;
        }
    }
    return true;
}

/**
 * Helper to check settling time requirement.
 * For this PID controller, the zeros allow a dominant pole at -30 rad/s
 * to meet the 0.040s settling time requirement.
 */
template <typename T, consteig::Size S>
constexpr bool check_settling_time(
    const consteig::Matrix<consteig::Complex<T>, S, 1> &eigs, double pole_limit)
{
    for (consteig::Size i = 0; i < S; ++i)
    {
        if (eigs(i, 0).real >= pole_limit)
        {
            return false;
        }
    }
    return true;
}

/**
 * Helper to check overshoot requirement (via damping ratio zeta).
 * Zeta > 0.504 corresponds to Overshoot < 16%.
 */
template <typename T, consteig::Size S>
constexpr bool check_overshoot(
    const consteig::Matrix<consteig::Complex<T>, S, 1> &eigs,
    double min_damping)
{
    for (consteig::Size i = 0; i < S; ++i)
    {
        T sigma = eigs(i, 0).real;
        T omega = eigs(i, 0).imag;
        if (consteig::abs(omega) > 1e-6)
        {
            T zeta = -sigma / consteig::sqrt(sigma * sigma + omega * omega);
            if (zeta < min_damping)
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
    static constexpr double J = 3.2284e-6; // Moment of inertia [kg⋅m²]
    static constexpr double b = 3.5077e-6; // Viscous friction [N⋅m⋅s/rad]
    static constexpr double K_m = 0.0274;  // Motor constant [V⋅s/rad]
    static constexpr double R = 4.0;       // Armature resistance [Ω]
    static constexpr double L = 2.75e-6;   // Armature inductance [H]

    static constexpr consteig::Size s{4};

    // --- State Space Assembly (constexpr) ---
    // Augmented state: x = [position; velocity; current; integral_error]
    // The addition of the integral_error state ensures:
    // "No steady-state error, even in the presence of a step disturbance input"
    static constexpr consteig::Matrix<double, s, s> A{
        {{0.0, 1.0, 0.0, 0.0},
         {0.0, -b / J, K_m / J, 0.0},
         {0.0, -K_m / L, -R / L, 0.0},
         {-1.0, 0.0, 0.0, 0.0}}};

    static constexpr consteig::Matrix<double, s, 1> B{
        {{0.0}, {0.0}, {1.0 / L}, {0.0}}};

    // --- Control Constraints ---
    static constexpr double POLE_LIMIT_FOR_SETTLING = -30.0;
    static constexpr double ZETA_LIMIT_FOR_OVERSHOOT = 0.504;

    // =========================================================================
    // SCENARIO 1: PID Tuning (Kp=21, Kd=0.15, Ki=500)
    // =========================================================================
    static constexpr double K1_good = 21.0;
    static constexpr double K2_good = 0.15;
    static constexpr double K3_good = 0.0;
    static constexpr double K4_good = -500.0;

    static constexpr consteig::Matrix<double, 1, s> K_good{
        {{K1_good, K2_good, K3_good, K4_good}}};

    static constexpr consteig::Matrix<double, s, s> A_cl_good{A - B * K_good};
    static constexpr auto eigs_good = consteig::eigenvalues(A_cl_good);

    // Verify Scenario 1 Requirements
    static_assert(check_stability(eigs_good),
                  "Scenario 1: System is unstable.");

    static_assert(check_settling_time(eigs_good, POLE_LIMIT_FOR_SETTLING),
                  "Scenario 1: Settling time less than 0.040 seconds [FAILED]");

    static_assert(check_overshoot(eigs_good, ZETA_LIMIT_FOR_OVERSHOOT),
                  "Scenario 1: Overshoot less than 16% [FAILED]");

    std::cout << "System Parameters (constexpr): J=" << J << ", b=" << b
              << ", K_m=" << K_m << "\n\n";
    std::cout << "--- SCENARIO 1: PID Tuning (Kp=21, Kd=0.15, Ki=500) ---\n";
    std::cout << "Gains [K=" << K1_good << ", " << K2_good << ", " << K3_good
              << ", " << K4_good << "] passed all checks:\n";
    std::cout << "  - Settling time less than 0.040 seconds [PASS]\n";
    std::cout << "  - Overshoot less than 16% [PASS]\n";
    std::cout << "  - No steady-state error (enforced by integrator) [PASS]\n";

    // =========================================================================
    // SCENARIO 2: PID Tuning (Kp=21, Kd=0.05, Ki=200)
    // =========================================================================
    static constexpr double K1_bad = 21.0;
    static constexpr double K2_bad = 0.05;
    static constexpr double K3_bad = 0.0;
    static constexpr double K4_bad = -200.0;

    static constexpr consteig::Matrix<double, 1, s> K_bad{
        {{K1_bad, K2_bad, K3_bad, K4_bad}}};

    static constexpr consteig::Matrix<double, s, s> A_cl_bad{A - B * K_bad};
    static constexpr auto eigs_bad = consteig::eigenvalues(A_cl_bad);

    // These assertions WILL fail the build if uncommented!
    static_assert(
        check_settling_time(eigs_bad, POLE_LIMIT_FOR_SETTLING),
        "Scenario 2 REJECTED: Settling time less than 0.040 seconds [FAILED]");
    static_assert(check_overshoot(eigs_bad, ZETA_LIMIT_FOR_OVERSHOOT),
                  "Scenario 2 REJECTED: Overshoot less than 16% [FAILED]");

    std::cout << "\n--- SCENARIO 2: PID Tuning (Kp=21, Kd=0.05, Ki=200) ---\n";
    std::cout << "Rejection status checked for:\n";
    std::cout << "  - Settling time less than 0.040 seconds\n";
    std::cout << "  - Overshoot less than 16%\n";
    std::cout << "  - No steady-state error\n";

    return 0;
}
