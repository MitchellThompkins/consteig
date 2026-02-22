#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, hamiltonian_10x10_28) { static_assert(check_single_hamiltonian_nonsym_10x10<28>(), "Test hamiltonian_10x10_28 failed"); SUCCEED(); }
#endif
