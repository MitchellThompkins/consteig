#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, random_non_normal_8x8_45) {
    static_assert(check_single_random_non_normal_nonsym_8x8<45>(),
                  "Test random_non_normal_8x8_45 failed");
    SUCCEED();
}
#endif
