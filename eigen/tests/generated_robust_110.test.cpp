#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, defective_fast_5) { static_assert(check_single_defective_nonsym_fast<5>(), "Test defective_fast_5 failed"); SUCCEED(); }
#endif
#endif
