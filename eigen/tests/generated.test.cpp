#include <gtest/gtest.h>
#include "test_tools.hpp"
#include "../consteig.hpp"
#include "generated_cases.hpp" // Included from test_dependencies

using namespace consteig;

template<typename T, Size S>
constexpr Complex<T> sum_eigs(const Matrix<Complex<T>, S, 1>& vec) {
    Complex<T> s{};
    for(Size i=0; i<S; ++i) s = s + vec(i,0);
    return s;
}

TEST(generated_tests, symmetric_constexpr)
{
    static constexpr auto eigs = eigvals(mat_sym);
    static constexpr double tr = trace(mat_sym);
    static constexpr auto s = sum_eigs(eigs);
    
    // Check Trace
    static_assert(consteig::abs(s.real - tr) < 1e-4, "Compile-time trace mismatch");
    static_assert(consteig::abs(s.imag) < 1e-9, "Compile-time imag mismatch");
    
    SUCCEED();
}

TEST(generated_tests, nonsymmetric_constexpr)
{
    static constexpr auto eigs = eigvals(mat_nonsym);
    static constexpr double tr = trace(mat_nonsym);
    static constexpr auto s = sum_eigs(eigs);
    
    // Check Trace
    static_assert(consteig::abs(s.real - tr) < 1e-4, "Compile-time trace mismatch");
    
    SUCCEED();
}

TEST(generated_tests, qr_constexpr)
{
    static constexpr QRMatrix<double, 4> res = qr(mat_qr);
    static constexpr Matrix<double, 4, 4> recon = res._q * res._r;
    
    // Verify A = Q*R property
    for(Size i=0; i<4; ++i) {
        for(Size j=0; j<4; ++j) {
            ASSERT_NEAR(recon(i,j), mat_qr(i,j), 1e-4);
        }
    }
    
    // Static assert check (approximate equality might require looser threshold or custom checker for static_assert)
    // We'll rely on the fact that 'res' was computed constexpr.
}
