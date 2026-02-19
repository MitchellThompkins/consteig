#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, hamiltonian_fast_4) { static_assert(check_single_hamiltonian_nonsym_fast<4>(), "Test hamiltonian_fast_4 failed"); SUCCEED(); }
#endif
#endif
