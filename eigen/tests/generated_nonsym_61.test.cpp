#include "generated_test_helpers.hpp"
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_slow_11) { static_assert(check_single_random_nonsym_slow<11>(), "Test random_slow_11 failed"); SUCCEED(); }
#endif
