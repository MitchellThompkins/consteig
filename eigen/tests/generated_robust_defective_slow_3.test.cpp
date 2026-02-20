#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, defective_slow_3) {
    static_assert(check_single_defective_nonsym_slow<3>(), "Test defective_slow_3 failed");
    SUCCEED();
}
#endif
#endif
