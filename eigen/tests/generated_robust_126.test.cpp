#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nearly_defective_fast_3) {
    static_assert(check_single_nearly_defective_nonsym_fast<3>(), "Test nearly_defective_fast_3 failed");
    SUCCEED();
}
#endif
#endif
