#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, hamiltonian_slow_6) {
    EXPECT_TRUE(check_single_hamiltonian_nonsym_slow<6>()) << "Test hamiltonian_slow_6 failed";
}
#endif
#endif
