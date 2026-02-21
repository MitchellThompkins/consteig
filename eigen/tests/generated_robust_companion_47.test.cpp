#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, companion_8x8_47) {
    static_assert(check_single_companion_nonsym_8x8<47>(),
                  "Test companion_8x8_47 failed");
    SUCCEED();
}
#endif
