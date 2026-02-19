#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, repeated_slow_6) { static_assert(check_single_repeated_nonsym_slow<6>(), "Test repeated_slow_6 failed"); SUCCEED(); }
#endif
#endif
