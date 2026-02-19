#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, hamiltonian_slow_5) { EXPECT_TRUE(check_single_hamiltonian_nonsym_slow<5>()) << "Test hamiltonian_slow_5 failed"; }
#endif
#endif
