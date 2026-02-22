#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, hamiltonian_10x10_22)
{
    static_assert(check_single_hamiltonian_nonsym_10x10<22>(),
                  "Test hamiltonian_10x10_22 failed");
    SUCCEED();
}
#endif
