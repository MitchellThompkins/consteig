#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nearly_reducible_fast_3) { static_assert(check_single_nearly_reducible_nonsym_fast<3>(), "Test nearly_reducible_fast_3 failed"); SUCCEED(); }
#endif
#endif
