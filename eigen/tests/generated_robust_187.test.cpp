#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, repeated_slow_3) { static_assert(check_single_repeated_nonsym_slow<3>(), "Test repeated_slow_3 failed"); SUCCEED(); }
#endif
#endif
