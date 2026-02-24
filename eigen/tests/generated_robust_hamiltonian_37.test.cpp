#include "generated_test_helpers.hpp"
TEST(generated_tests, hamiltonian_8x8_37)
{
    static_assert(check_single_hamiltonian_nonsym_8x8<37>(),
                  "Test hamiltonian_8x8_37 failed");
    SUCCEED();
}
