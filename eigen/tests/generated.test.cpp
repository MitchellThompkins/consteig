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

template<typename T, Size S>
constexpr bool verify_eigenvalues(const Matrix<Complex<T>, S, 1>& computed, const Matrix<Complex<T>, S, 1>& expected, T tol = static_cast<T>(CONSTEIG_TEST_TOLERANCE)) {
    bool matched[S] = {};
    for(Size i=0; i<S; ++i) matched[i] = false;
    
    for(Size i=0; i<S; ++i) {
        bool found = false;
        for(Size j=0; j<S; ++j) {
            if(!matched[j]) {
                if(consteig::abs(computed(i,0) - expected(j,0)) < tol) {
                    matched[j] = true;
                    found = true;
                    break;
                }
            }
        }
        if(!found) return false;
    }
    return true;
}

TEST(generated_tests, symmetric_constexpr)
{
    static constexpr auto eigs = eigvals(mat_sym);
    static constexpr double tr = trace(mat_sym);
    static constexpr auto s = sum_eigs(eigs);
    
    // Check Trace
    static_assert(consteig::abs(s.real - tr) < static_cast<double>(CONSTEIG_TEST_TOLERANCE), "Compile-time trace mismatch");
    static_assert(consteig::abs(s.imag) < 1e-9, "Compile-time imag mismatch");

    // Check Eigenvalues
    static_assert(verify_eigenvalues(eigs, eigs_sym), "Compile-time eigenvalues mismatch (symmetric)");
    
    SUCCEED();
}

TEST(generated_tests, nonsymmetric_constexpr)
{
    static constexpr auto eigs = eigvals(mat_nonsym);
    static constexpr double tr = trace(mat_nonsym);
    static constexpr auto s = sum_eigs(eigs);
    
    // Check Trace
    static_assert(consteig::abs(s.real - tr) < static_cast<double>(CONSTEIG_TEST_TOLERANCE), "Compile-time trace mismatch");

    // Check Eigenvalues
    static_assert(verify_eigenvalues(eigs, eigs_nonsym), "Compile-time eigenvalues mismatch (non-symmetric)");

    SUCCEED();
}

TEST(generated_tests, qr_constexpr)
{
    static constexpr QRMatrix<double, 4> res = qr(mat_qr);
    static constexpr Matrix<double, 4, 4> recon = res._q * res._r;
    
    // Verify A = Q*R property
    static_assert(compareFloatMat(recon, mat_qr, static_cast<double>(CONSTEIG_TEST_TOLERANCE)), "QR Reconstruction failed (constexpr)");

    // Verify Q is unitary
    static constexpr Matrix<double, 4, 4> qUnitary = transpose(res._q) * res._q;
    static constexpr Matrix<double, 4, 4> identity = eye<double, 4>();
    static_assert(compareFloatMat(qUnitary, identity, static_cast<double>(CONSTEIG_TEST_TOLERANCE)), "Q not unitary (constexpr)");

    for(Size i=0; i<4; ++i) {
        for(Size j=0; j<4; ++j) {
            ASSERT_NEAR(recon(i,j), mat_qr(i,j), static_cast<double>(CONSTEIG_TEST_TOLERANCE));
        }
    }
}
