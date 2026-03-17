// Minimal compilation unit for cross-compiler verification.
// Includes the full library and exercises key template instantiations
// so the compiler checks the freestanding code paths.
#include "consteig.hpp"

namespace cross_verify {

using namespace consteig;

// Matrix creation and basic operations
constexpr Matrix<double, 3, 3> A{
    {{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 10.0}}}};

constexpr Matrix<double, 3, 3> B{
    {{{10.0, 9.0, 8.0}, {7.0, 6.0, 5.0}, {4.0, 3.0, 1.0}}}};

// Arithmetic
constexpr auto sum = A + B;
constexpr auto diff = A - B;
constexpr auto prod = A * B;
constexpr auto At = transpose(A);
constexpr auto scaled = 2.0 * A;

// Determinant
constexpr auto det_A = det(A);
static_assert(compareFloats(det_A, -3.0, 1e-9), "determinant check");

// Identity
constexpr auto I = eye<double, 3>();
static_assert(compareFloats(I(0, 0), 1.0, 1e-9), "identity check");

// Eigenvalue computation (double)
constexpr auto eig_d = eigvals(A);
static_assert(eig_d(0, 0).real != 0.0, "eigenvalue sanity check");

// Eigenvalue computation (float)
constexpr Matrix<float, 2, 2> F{{{{1.0f, 2.0f}, {3.0f, 4.0f}}}};
constexpr auto eig_f = eigvals(F);
static_assert(eig_f(0, 0).real != 0.0f, "float eigenvalue sanity check");

// Use remaining results in static_asserts to suppress unused warnings
static_assert(sum(0, 0) == 11.0, "addition check");
static_assert(diff(0, 0) == -9.0, "subtraction check");
static_assert(prod(0, 0) != 0.0, "multiplication check");
static_assert(At(0, 1) == 4.0, "transpose check");
static_assert(scaled(0, 0) == 2.0, "scalar multiply check");

} // namespace cross_verify
