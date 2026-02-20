#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nearly_defective_fast_2) {
    static_assert(check_single_nearly_defective_nonsym_fast<2>(), "Test nearly_defective_fast_2 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nearly_defective_slow_2) {
    static_assert(check_single_nearly_defective_nonsym_slow<2>(), "Test nearly_defective_slow_2 failed");
    SUCCEED();
}
#endif
#endif
