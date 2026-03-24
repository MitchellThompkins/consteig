#include "generated_test_helpers.hpp"
TEST(generated_tests, hamiltonian_8x8_19)
{
    static_assert(check_single_hamiltonian_nonsym_8x8<19>(),
                  "Test hamiltonian_8x8_19 failed");
    SUCCEED();
}
