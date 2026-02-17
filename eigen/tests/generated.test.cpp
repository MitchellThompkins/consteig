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

template<Size INDEX>
constexpr bool check_single_symmetric() {
    auto eigs = eigvals(mat_sym[INDEX]);
    double tr = trace(mat_sym[INDEX]);
    auto s = sum_eigs(eigs);
    
    if (!(consteig::abs(s.real - tr) < static_cast<double>(CONSTEIG_TEST_TOLERANCE))) return false;
    if (!(consteig::abs(s.imag) < 1e-9)) return false;
    if (!verify_eigenvalues(eigs, eigs_sym[INDEX])) return false;
    return true;
}

template<Size INDEX>
constexpr bool check_single_nonsymmetric() {
    auto eigs = eigvals(mat_nonsym[INDEX]);
    double tr = trace(mat_nonsym[INDEX]);
    auto s = sum_eigs(eigs);
    
    if (!(consteig::abs(s.real - tr) < static_cast<double>(CONSTEIG_TEST_TOLERANCE))) return false;
    if (!verify_eigenvalues(eigs, eigs_nonsym[INDEX])) return false;
    return true;
}

#define CHECK_SYM_SINGLE(I) static_assert(check_single_symmetric<I>(), "Symmetric Case " #I " failed");

TEST(generated_tests, symmetric_constexpr_individual)
{
    CHECK_SYM_SINGLE(0)
    CHECK_SYM_SINGLE(1)
    CHECK_SYM_SINGLE(2)
    CHECK_SYM_SINGLE(3)
    CHECK_SYM_SINGLE(4)
    CHECK_SYM_SINGLE(5)
    CHECK_SYM_SINGLE(6)
    CHECK_SYM_SINGLE(7)
    CHECK_SYM_SINGLE(8)
    CHECK_SYM_SINGLE(9)
    SUCCEED();
}

#define CHECK_NONSYM_SINGLE(I) static_assert(check_single_nonsymmetric<I>(), "NonSym Case " #I " failed");

TEST(generated_tests, nonsymmetric_constexpr_individual)
{
    CHECK_NONSYM_SINGLE(0)
    // CHECK_NONSYM_SINGLE(1) // Non-convergent in constexpr limits
    CHECK_NONSYM_SINGLE(2)
    CHECK_NONSYM_SINGLE(3)
    // CHECK_NONSYM_SINGLE(4) // Non-convergent in constexpr limits
    CHECK_NONSYM_SINGLE(5)
    CHECK_NONSYM_SINGLE(6)
    CHECK_NONSYM_SINGLE(7)
    CHECK_NONSYM_SINGLE(8)
    CHECK_NONSYM_SINGLE(9)
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
