#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, defective_8x8_48) {
    static_assert(check_single_defective_nonsym_8x8<48>(),
                  "Test defective_8x8_48 failed");
    SUCCEED();
}
#endif
