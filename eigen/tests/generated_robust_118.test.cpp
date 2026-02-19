#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, defective_fast_9) { static_assert(check_single_defective_nonsym_fast<9>(), "Test defective_fast_9 failed"); SUCCEED(); }
#endif
#endif
