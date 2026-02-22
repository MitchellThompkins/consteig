#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, hamiltonian_10x10_4) { static_assert(check_single_hamiltonian_nonsym_10x10<4>(), "Test hamiltonian_10x10_4 failed"); SUCCEED(); }
#endif
