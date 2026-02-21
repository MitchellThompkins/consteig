#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, random_non_normal_8x8_19) {
    static_assert(check_single_random_non_normal_nonsym_8x8<19>(),
                  "Test random_non_normal_8x8_19 failed");
    SUCCEED();
}
#endif
