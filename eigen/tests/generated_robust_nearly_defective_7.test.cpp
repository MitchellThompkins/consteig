#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nearly_defective_fast_7) { static_assert(check_single_nearly_defective_nonsym_fast<7>(), "Test nearly_defective_fast_7 failed"); SUCCEED(); }
#else
TEST(generated_tests, nearly_defective_slow_7) { static_assert(check_single_nearly_defective_nonsym_slow<7>(), "Test nearly_defective_slow_7 failed"); SUCCEED(); }
#endif
#endif
