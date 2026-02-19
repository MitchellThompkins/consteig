#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, hamiltonian_fast_1) {
    EXPECT_TRUE(check_single_hamiltonian_nonsym_fast<1>()) << "Test hamiltonian_fast_1 failed";
}
#endif
#endif
