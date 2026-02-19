#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, companion_slow_4) { static_assert(check_single_companion_nonsym_slow<4>(), "Test companion_slow_4 failed"); SUCCEED(); }
#endif
#endif
