#include "generated_test_helpers.hpp"
TEST(generated_tests, hamiltonian_8x8_8)
{
    static_assert(check_single_hamiltonian_nonsym_8x8<8>(), "Test hamiltonian_8x8_8 failed");
    SUCCEED();
}
