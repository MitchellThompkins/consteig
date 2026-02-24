#include "generated_test_helpers.hpp"
TEST(generated_tests, hamiltonian_8x8_48) { static_assert(check_single_hamiltonian_nonsym_8x8<48>(), "Test hamiltonian_8x8_48 failed"); SUCCEED(); }
