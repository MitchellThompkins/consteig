#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, non_normal_fast_6) { static_assert(check_single_non_normal_nonsym_fast<6>(), "Test non_normal_fast_6 failed"); SUCCEED(); }
#endif
#endif
