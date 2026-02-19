#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, hamiltonian_fast_8) { EXPECT_TRUE(check_single_hamiltonian_nonsym_fast<8>()) << "Test hamiltonian_fast_8 failed"; }
#endif
#endif
