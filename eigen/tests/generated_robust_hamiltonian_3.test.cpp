#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, hamiltonian_fast_3) { static_assert(check_single_hamiltonian_nonsym_fast<3>(), "Test hamiltonian_fast_3 failed"); SUCCEED(); }
#else
TEST(generated_tests, hamiltonian_slow_3) { static_assert(check_single_hamiltonian_nonsym_slow<3>(), "Test hamiltonian_slow_3 failed"); SUCCEED(); }
#endif
#endif
