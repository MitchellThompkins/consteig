#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nearly_reducible_slow_2) { static_assert(check_single_nearly_reducible_nonsym_slow<2>(), "Test nearly_reducible_slow_2 failed"); SUCCEED(); }
#endif
#endif
