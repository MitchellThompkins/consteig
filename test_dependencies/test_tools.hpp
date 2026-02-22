#ifndef TEST_TOOLS_HPP
#define TEST_TOOLS_HPP

#include <cfloat>
#include <limits>

#include "../consteig.hpp"
#include "../consteig_options.hpp"

#define MSG "Not constexpr or wrong output"

// Use a slightly looser tolerance for tests involving iterative methods (QR,
// Eigen) compared to the strict symmetry check tolerance.
#ifndef CONSTEIG_TEST_TOLERANCE
#define CONSTEIG_TEST_TOLERANCE 1e-9F
#endif

// It's possible that some of the more diff ult matrices require looser test
// tolerances
#ifndef RANDOM_TOL
#define RANDOM_TOL CONSTEIG_TEST_TOLERANCE
#endif

#ifndef STRICT_TOL
#define STRICT_TOL CONSTEIG_TEST_TOLERANCE
#endif

#ifndef PATHOLOGICAL_TOL
#define PATHOLOGICAL_TOL 0.03
#endif

// https://stackoverflow.com/a/32334103/3527182
template <typename T>
constexpr bool nearlyEqual(T a, T b, T epsilon = 128 * FLT_EPSILON,
                           T relth = FLT_MIN)
// those defaults are arbitrary and could be removed
{
    static_assert(consteig::is_float<T>(), "Expects floating point number");
    assert(std::numeric_limits<T>::epsilon() <= epsilon);
    assert(epsilon < 1.f);

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

#endif
