#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, hamiltonian_8x8_21)
{
    static_assert(check_single_hamiltonian_nonsym_8x8<21>(),
                  "Test hamiltonian_8x8_21 failed");
    SUCCEED();
}
#endif
