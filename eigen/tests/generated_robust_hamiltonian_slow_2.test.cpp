#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, hamiltonian_slow_2) {
    static_assert(check_single_hamiltonian_nonsym_slow<2>(), "Test hamiltonian_slow_2 failed");
    SUCCEED();
}
#endif
#endif
