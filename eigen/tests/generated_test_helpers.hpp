#ifndef GENERATED_TEST_HELPERS_HPP
#define GENERATED_TEST_HELPERS_HPP

#include <gtest/gtest.h>

#include "../consteig.hpp"
#include "generated_cases.hpp"
#include "test_tools.hpp"

using namespace consteig;

// Define helper macros for consistency
#define GENERATE_CHECK(category, type, suffix, mat_var, eig_var, tol)                        \
    template <Size INDEX>                                                                    \
    constexpr bool check_single_##category##_##type##_##suffix() {                           \
        auto eigs = eigvals(mat_var[INDEX]);                                                 \
        if (!checkEigenValues(mat_var[INDEX], eigs, static_cast<double>(tol))) return false; \
        if (!compareEigenValues(eigs, eig_var[INDEX], static_cast<double>(tol))) return false; \
        return true;                                                                         \
    }

// Random cases
GENERATE_CHECK(random, sym, fast, mat_random_sym_fast, eigs_random_sym_fast, CONSTEIG_TEST_TOLERANCE)
GENERATE_CHECK(random, nonsym, fast, mat_random_nonsym_fast, eigs_random_nonsym_fast, CONSTEIG_TEST_TOLERANCE)
GENERATE_CHECK(random, sym, slow, mat_random_sym_slow, eigs_random_sym_slow, CONSTEIG_TEST_TOLERANCE)
GENERATE_CHECK(random, nonsym, slow, mat_random_nonsym_slow, eigs_random_nonsym_slow, CONSTEIG_TEST_TOLERANCE)

// Robustness cases
#define ROBUST_TOL CONSTEIG_TEST_TOLERANCE

#define GENERATE_ROBUST(category)                                                                                   \
    GENERATE_CHECK(category, nonsym, fast, mat_##category##_nonsym_fast, eigs_##category##_nonsym_fast, ROBUST_TOL) \
    GENERATE_CHECK(category, nonsym, slow, mat_##category##_nonsym_slow, eigs_##category##_nonsym_slow, ROBUST_TOL)

GENERATE_ROBUST(defective)
GENERATE_ROBUST(nearly_defective)
GENERATE_ROBUST(non_normal)
GENERATE_ROBUST(clustered)
GENERATE_ROBUST(repeated)
GENERATE_ROBUST(companion)
GENERATE_ROBUST(graded)
GENERATE_ROBUST(large_jordan)
GENERATE_ROBUST(toeplitz)
GENERATE_ROBUST(nearly_reducible)
GENERATE_ROBUST(random_non_normal)
GENERATE_ROBUST(hamiltonian)
GENERATE_ROBUST(sparse_interior)

#endif
