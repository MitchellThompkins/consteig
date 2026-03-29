#ifndef GENERATED_TEST_HELPERS_HPP
#define GENERATED_TEST_HELPERS_HPP

#include <gtest/gtest.h>

#include <consteig/consteig.hpp>
#include "generated_cases.hpp"
#include "test_tools.hpp"

using namespace consteig;

// Define helper macros for consistency
#define GENERATE_CHECK(category, type, suffix, mat_var, eig_var, tol)          \
    template <Size INDEX>                                                      \
    constexpr bool check_single_##category##_##type##_##suffix()               \
    {                                                                          \
        auto eigs = eigenvalues(mat_var[INDEX]);                               \
        if (!checkEigenValues(mat_var[INDEX], eigs, static_cast<double>(tol))) \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        if (!compareEigenValues(eigs, eig_var[INDEX],                          \
                                static_cast<double>(tol)))                     \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        auto vecs = eigenvectors(mat_var[INDEX], eigs);                        \
        if (!checkEigenVectorsInvariant(mat_var[INDEX], eigs, vecs,            \
                                        static_cast<double>(tol)))             \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        return true;                                                           \
    }

// Random cases
GENERATE_CHECK(random, sym, 8x8, mat_random_sym_8x8, eigs_random_sym_8x8,
               RANDOM_TOL)
GENERATE_CHECK(random, nonsym, 8x8, mat_random_nonsym_8x8,
               eigs_random_nonsym_8x8, RANDOM_TOL)

// Robustness cases
#define GENERATE_ROBUST(category, tol)                                         \
    GENERATE_CHECK(category, nonsym, 8x8, mat_##category##_nonsym_8x8,         \
                   eigs_##category##_nonsym_8x8, tol)

GENERATE_ROBUST(defective, PATHOLOGICAL_TOL)
GENERATE_ROBUST(nearly_defective, PATHOLOGICAL_TOL)
GENERATE_ROBUST(large_jordan, PATHOLOGICAL_TOL)

GENERATE_ROBUST(non_normal, STRICT_TOL)
GENERATE_ROBUST(clustered, STRICT_TOL)
GENERATE_ROBUST(repeated, STRICT_TOL)
GENERATE_ROBUST(companion, STRICT_TOL)
GENERATE_ROBUST(graded, STRICT_TOL)
GENERATE_ROBUST(toeplitz, STRICT_TOL)
GENERATE_ROBUST(nearly_reducible, STRICT_TOL)
GENERATE_ROBUST(random_non_normal, STRICT_TOL)
GENERATE_ROBUST(hamiltonian, STRICT_TOL)
GENERATE_ROBUST(sparse_interior, STRICT_TOL)

#endif
