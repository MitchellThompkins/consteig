#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, hamiltonian_10x10_2)
{
    static_assert(check_single_hamiltonian_nonsym_10x10<2>(),
                  "Test hamiltonian_10x10_2 failed");
    SUCCEED();
}
#endif
