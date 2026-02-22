#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, graded_8x8_30) { static_assert(check_single_graded_nonsym_8x8<30>(), "Test graded_8x8_30 failed"); SUCCEED(); }
#endif
