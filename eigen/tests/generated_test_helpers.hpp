#ifndef GENERATED_TEST_HELPERS_HPP
#define GENERATED_TEST_HELPERS_HPP

#include <gtest/gtest.h>

#include "../consteig.hpp"
#include "generated_cases.hpp"
#include "test_tools.hpp"

using namespace consteig;

template <typename T, Size S>
constexpr Complex<T> sum_eigs(const Matrix<Complex<T>, S, 1>& vec) {
    Complex<T> s{};
    for (Size i = 0; i < S; ++i) s = s + vec(i, 0);
    return s;
}

template <typename T, Size S>
constexpr bool verify_eigenvalues(const Matrix<Complex<T>, S, 1>& computed, const Matrix<Complex<T>, S, 1>& expected,
                                  T tol = static_cast<T>(CONSTEIG_TEST_TOLERANCE)) {
    bool matched[S] = {};
    for (Size i = 0; i < S; ++i) matched[i] = false;

    for (Size i = 0; i < S; ++i) {
        bool found = false;
        for (Size j = 0; j < S; ++j) {
            if (!matched[j]) {
                if (consteig::abs(computed(i, 0) - expected(j, 0)) < tol) {
                    matched[j] = true;
                    found = true;
                    break;
                }
            }
        }
        if (!found) return false;
    }
    return true;
}

template <Size INDEX>
constexpr bool check_single_symmetric_fast() {
    auto eigs = eigvals(mat_sym_fast[INDEX]);
    double tr = trace(mat_sym_fast[INDEX]);
    auto s = sum_eigs(eigs);

    if (!(consteig::abs(s.real - tr) < static_cast<double>(CONSTEIG_TEST_TOLERANCE))) return false;
    if (!(consteig::abs(s.imag) < 1e-9)) return false;
    if (!verify_eigenvalues(eigs, eigs_sym_fast[INDEX])) return false;
    return true;
}

template <Size INDEX>
constexpr bool check_single_nonsymmetric_fast() {
    auto eigs = eigvals(mat_nonsym_fast[INDEX]);
    double tr = trace(mat_nonsym_fast[INDEX]);
    auto s = sum_eigs(eigs);

    if (!(consteig::abs(s.real - tr) < static_cast<double>(CONSTEIG_TEST_TOLERANCE))) return false;
    if (!verify_eigenvalues(eigs, eigs_nonsym_fast[INDEX])) return false;
    return true;
}

template <Size INDEX>
constexpr bool check_single_symmetric_slow() {
    auto eigs = eigvals(mat_sym_slow[INDEX]);
    double tr = trace(mat_sym_slow[INDEX]);
    auto s = sum_eigs(eigs);

    if (!(consteig::abs(s.real - tr) < static_cast<double>(CONSTEIG_TEST_TOLERANCE))) return false;
    if (!(consteig::abs(s.imag) < 1e-9)) return false;
    if (!verify_eigenvalues(eigs, eigs_sym_slow[INDEX])) return false;
    return true;
}

template <Size INDEX>
constexpr bool check_single_nonsymmetric_slow() {
    auto eigs = eigvals(mat_nonsym_slow[INDEX]);
    double tr = trace(mat_nonsym_slow[INDEX]);
    auto s = sum_eigs(eigs);

    if (!(consteig::abs(s.real - tr) < static_cast<double>(CONSTEIG_TEST_TOLERANCE))) return false;
    if (!verify_eigenvalues(eigs, eigs_nonsym_slow[INDEX])) return false;
    return true;
}

#endif
