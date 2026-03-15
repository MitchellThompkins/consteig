#ifndef TEST_TOOLS_HPP
#define TEST_TOOLS_HPP

#include <cfloat>
#include <limits>

#include "../consteig.hpp"
#include "../consteig_options.hpp"

#define MSG "Not constexpr or wrong output"

// Standard tolerance used to compare against double precision iterative and
// math operations
#ifndef CONSTEIG_TEST_TOLERANCE
#define CONSTEIG_TEST_TOLERANCE 1e-9
#endif

// Tolerance specific to 32-bit floating point precision
#ifndef CONSTEIG_FLOAT_TEST_TOLERANCE
#define CONSTEIG_FLOAT_TEST_TOLERANCE 1e-7f
#endif

// Tolerance for complex iterative methods using 32-bit floats
// (e.g., Hessenberg reduction) where error accumulation is significant.
#ifndef CONSTEIG_ITERATIVE_FLOAT_TOLERANCE
#define CONSTEIG_ITERATIVE_FLOAT_TOLERANCE 3e-4f
#endif

// Tolerance for iterative methods using 64-bit doubles
// (e.g., Hessenberg reduction) where error accumulation is significant.
#ifndef CONSTEIG_ITERATIVE_DOUBLE_TOLERANCE
#define CONSTEIG_ITERATIVE_DOUBLE_TOLERANCE 3e-4
#endif

// It's possible that some of the more difficult matrices require looser test
// tolerances. Observationally, that has only shown to be the case for highly
// defective matrices.
#ifndef RANDOM_TOL
#define RANDOM_TOL CONSTEIG_TEST_TOLERANCE
#endif

#ifndef STRICT_TOL
#define STRICT_TOL CONSTEIG_TEST_TOLERANCE
#endif

#ifndef PATHOLOGICAL_TOL
#define PATHOLOGICAL_TOL 0.03
#endif

// Specific tolerance for very large value comparisons (e.g., 1e10).
// While 1.0 seems large, it represents high precision (~1e-10 relative error)
// for high-magnitude numbers.
#ifndef LARGE_VAL_TOL
#define LARGE_VAL_TOL 1.0
#endif

// Large sentinel value for initializing minimum distance searches
#ifndef MAX_SENTINEL_VAL
#define MAX_SENTINEL_VAL 1e12
#endif

// https://stackoverflow.com/a/32334103/3527182
template <typename T>
constexpr bool nearlyEqual(T a, T b, T epsilon = 128 * FLT_EPSILON,
                           T relth = FLT_MIN)
// those defaults are arbitrary and could be removed
{
    static_assert(consteig::is_float<T>(), "Expects floating point number");
    assert(std::numeric_limits<T>::epsilon() <= epsilon);
    assert(epsilon < T(1));

    if (a == b)
        return true;

    auto diff = std::abs(a - b);
    auto norm =
        std::min((std::abs(a) + std::abs(b)), std::numeric_limits<T>::max());
    // or even faster: std::min(std::abs(a + b),
    // std::numeric_limits<float>::max()); keeping this commented out until I
    // update figures below
    return diff < std::max(relth, epsilon * norm);
}

template <typename T, consteig::Size R, consteig::Size C>
static constexpr bool compareFloatMat(consteig::Matrix<T, R, C> a,
                                      consteig::Matrix<T, R, C> b,
                                      const T thresh)
{
    for (consteig::Size i{0}; i < R; i++)
    {
        for (consteig::Size j{0}; j < C; j++)
        {
            if (!consteig::compareFloats(a(i, j), b(i, j), thresh))
                return false;
        }
    }
    return true;
}

template <typename T, consteig::Size S>
static constexpr bool compareEigenValues(
    consteig::Matrix<consteig::Complex<T>, S, 1> a,
    consteig::Matrix<consteig::Complex<T>, S, 1> b, const T thresh)
{
    // a is computed, b is reference.
    // Set matching algorithm (O(N^2)) - robust against permutations and jitter.
    bool used[S] = {};
    for (consteig::Size i = 0; i < S; ++i)
        used[i] = false;

    for (consteig::Size i = 0; i < S; ++i)
    { // for each reference eigenvalue b[i]
        bool found = false;
        for (consteig::Size j = 0; j < S; ++j)
        { // find an unused match in a[j]
            if (!used[j])
            {
                const T diff_real = consteig::abs(a(j, 0).real - b(i, 0).real);
                const T diff_imag = consteig::abs(a(j, 0).imag - b(i, 0).imag);
                if (diff_real < thresh && diff_imag < thresh)
                {
                    used[j] = true;
                    found = true;
                    break;
                }
            }
        }
        if (!found)
            return false;
    }
    return true;
}

template <typename T, consteig::Size S>
static constexpr bool checkEigenVectorsInvariant(
    const consteig::Matrix<T, S, S> &A,
    const consteig::Matrix<consteig::Complex<T>, S, 1> &lambda,
    const consteig::Matrix<consteig::Complex<T>, S, S> &V, const T thresh)
{
    // For each eigenvalue / eigenvector pair...
    for (consteig::Size j = 0; j < S; ++j)
    {
        consteig::Complex<T> lam = lambda(j, 0);

        // Check A * v = lambda * v for the j-th column
        for (consteig::Size i = 0; i < S; ++i)
        {
            consteig::Complex<T> Av_i{0, 0};
            for (consteig::Size k = 0; k < S; ++k)
            {
                Av_i = Av_i + consteig::Complex<T>{A(i, k)} * V(k, j);
            }

            consteig::Complex<T> lv_i = lam * V(i, j);

            T diff_real = consteig::abs(Av_i.real - lv_i.real);
            T diff_imag = consteig::abs(Av_i.imag - lv_i.imag);

            if (diff_real > thresh || diff_imag > thresh)
            {
                return false;
            }
        }
    }
    return true;
}

#endif
