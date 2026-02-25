#include "../consteig.hpp"
#include "example_helper.hpp"
#include <iostream>

/**
 * Performance Requirements for DC Motor Control
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
constexpr bool check_performance(const consteig::Matrix<consteig::Complex<T>, S, 1>& eigs,
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

/**
 * Compile-time 3x3 Matrix Inverse using Adjugate Matrix
 */
template <typename T>
constexpr consteig::Matrix<T, 3, 3> inverse3x3(const consteig::Matrix<T, 3, 3>& mat)
{
    T d = consteig::det(mat);
    consteig::Matrix<T, 3, 3> inv{};
    
    inv(0, 0) = (mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1)) / d;
    inv(0, 1) = (mat(0, 2) * mat(2, 1) - mat(0, 1) * mat(2, 2)) / d;
    inv(0, 2) = (mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1)) / d;
    
    inv(1, 0) = (mat(1, 2) * mat(2, 0) - mat(1, 0) * mat(2, 2)) / d;
    inv(1, 1) = (mat(0, 0) * mat(2, 2) - mat(0, 2) * mat(2, 0)) / d;
    inv(1, 2) = (mat(0, 2) * mat(1, 0) - mat(0, 0) * mat(1, 2)) / d;
    
    inv(2, 0) = (mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0)) / d;
    inv(2, 1) = (mat(0, 1) * mat(2, 0) - mat(0, 0) * mat(2, 1)) / d;
    inv(2, 2) = (mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0)) / d;
    
    return inv;
}

/**
 * Compile-time Pole Placement using Ackermann's Formula for 3rd order system
 */
template <typename T>
constexpr consteig::Matrix<T, 1, 3> place(const consteig::Matrix<T, 3, 3>& A,
                                          const consteig::Matrix<T, 3, 1>& B,
                                          consteig::Complex<T> p1,
                                          consteig::Complex<T> p2,
                                          consteig::Complex<T> p3)
{
    // Characteristic polynomial: (s - p1)(s - p2)(s - p3) = s^3 + a2*s^2 + a1*s + a0
    consteig::Complex<T> sum_p = p1 + p2 + p3;
    consteig::Complex<T> a2_c{ -sum_p.real, -sum_p.imag };
    
    consteig::Complex<T> a1_c = (p1 * p2) + (p2 * p3) + (p1 * p3);
    
    consteig::Complex<T> prod_p = p1 * p2 * p3;
    consteig::Complex<T> a0_c{ -prod_p.real, -prod_p.imag };

    // Coefficients are real for physical systems (complex conjugate pairs)
    T a2 = a2_c.real;
    T a1 = a1_c.real;
    T a0 = a0_c.real;

    consteig::Matrix<T, 3, 3> A2 = A * A;
    consteig::Matrix<T, 3, 3> A3 = A2 * A;
    consteig::Matrix<T, 3, 3> I = consteig::eye<T, 3>();

    // Δ(A) = A^3 + a2*A^2 + a1*A + a0*I
    consteig::Matrix<T, 3, 3> deltaA = A3 + a2 * A2 + a1 * A + a0 * I;

    // Controllability matrix: C = [B, AB, A^2B]
    consteig::Matrix<T, 3, 3> C{};
    consteig::Matrix<T, 3, 1> AB = A * B;
    consteig::Matrix<T, 3, 1> A2B = A2 * B;

    for (consteig::Size i = 0; i < 3; ++i)
    {
        C(i, 0) = B(i, 0);
        C(i, 1) = AB(i, 0);
        C(i, 2) = A2B(i, 0);
    }

    consteig::Matrix<T, 3, 3> C_inv = inverse3x3(C);
    
    // e3 = [0, 0, 1]
    consteig::Matrix<T, 1, 3> e3{{{{0.0, 0.0, 1.0}}}};
    
    // K = e3 * C^-1 * Δ(A)
    return e3 * C_inv * deltaA;
}

int main()
{
    // --- Motor Parameters (Defined at compile-time) ---
    static constexpr double J = 0.01;    // Moment of inertia [kg⋅m²]
    static constexpr double b = 0.1;     // Viscous friction [N⋅m⋅s/rad]
    static constexpr double K_m = 0.01;  // Motor constant [V⋅s/rad]
    static constexpr double R = 1.0;     // Armature resistance [Ω]
    static constexpr double L = 0.5;     // Armature inductance [H]

    static constexpr consteig::Size s{3};

    // --- State Space Assembly (constexpr) ---
    static constexpr consteig::Matrix<double, s, s> A{
        {{{0.0, 1.0, 0.0},
          {0.0, -b / J, K_m / J},
          {0.0, -K_m / L, -R / L}}}};

    static constexpr consteig::Matrix<double, s, 1> B{
        {{{0.0},
          {0.0},
          {1.0 / L}}}};

    // --- Control Constraints ---
    static constexpr PerformanceRequirements limits = {
        -2.0,  // min_convergence: slowest pole allowed
        -50.0, // max_convergence: fastest pole allowed (limit current)
        0.55   // min_damping: avoid gearbox stress
    };

    // --- 1. GOOD CONTROL ---
    // Poles: [-5+4j, -5-4j, -6]
    static constexpr consteig::Complex<double> p_good1{-5.0, 4.0};
    static constexpr consteig::Complex<double> p_good2{-5.0, -4.0};
    static constexpr consteig::Complex<double> p_good3{-6.0, 0.0};

    // Compute K dynamically at compile-time using Ackermann's formula
    static constexpr consteig::Matrix<double, 1, s> K_good = place(A, B, p_good1, p_good2, p_good3);
    
    // Closed loop system A_cl = A - B*K
    static constexpr consteig::Matrix<double, s, s> A_cl_good{A - B * K_good};
    static constexpr consteig::Matrix<consteig::Complex<double>, s, 1> eigs_good{
        consteig::eigvals(A_cl_good)};

    // Compile-time assertion for performance validation
    static_assert(check_performance(eigs_good, limits),
                  "PERFORMANCE FAILURE: Good gains failed requirements!");

    std::cout << "System Parameters (constexpr): J=" << J << ", b=" << b << ", K_m=" << K_m << "\n";
    std::cout << "Computed good gains K = [" << K_good(0,0) << ", " << K_good(0,1) << ", " << K_good(0,2) << "]\n";
    std::cout << "Good closed loop poles:\n";
    for (consteig::Size i = 0; i < s; ++i)
    {
        std::cout << "  " << eigs_good(i, 0).real << " + " << eigs_good(i, 0).imag << "i\n";
    }

    // --- 2. UNDERDAMPED CONTROL ---
    // Poles: [-5+8j, -5-8j, -6]
    static constexpr consteig::Complex<double> p_bad1{-5.0, 8.0};
    static constexpr consteig::Complex<double> p_bad2{-5.0, -8.0};
    static constexpr consteig::Complex<double> p_bad3{-6.0, 0.0};

    static constexpr consteig::Matrix<double, 1, s> K_bad = place(A, B, p_bad1, p_bad2, p_bad3);
    static constexpr consteig::Matrix<double, s, s> A_cl_bad{A - B * K_bad};
    static constexpr consteig::Matrix<consteig::Complex<double>, s, 1> eigs_bad{
        consteig::eigvals(A_cl_bad)};

    // Prove Compile-Time Detection of Performance Violation
    // This will intentionally fail the compilation to demonstrate that
    // bad control gains are rejected by the compiler before the program can run!
    static constexpr bool bad_perf_ok = check_performance(eigs_bad, limits);
    static_assert(bad_perf_ok, "SYSTEM REJECTED: Underdamped system detected (damping ratio too low)!");

    std::cout << "\nComputed bad gains K = [" << K_bad(0,0) << ", " << K_bad(0,1) << ", " << K_bad(0,2) << "]\n";
    std::cout << "Bad gains rejected due to performance violation.\n";
    std::cout << "Bad closed loop poles:\n";
    for (consteig::Size i = 0; i < s; ++i)
    {
        double sigma = eigs_bad(i, 0).real;
        double omega = eigs_bad(i, 0).imag;
        std::cout << "  " << sigma << " + " << omega << "i";
        if (consteig::abs(omega) > 1e-6)
        {
            std::cout << " (zeta = " << -sigma / consteig::sqrt(sigma * sigma + omega * omega) << ")";
        }
        std::cout << "\n";
    }

    return 0;
}
