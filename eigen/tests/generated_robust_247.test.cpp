#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, large_jordan_slow_3) { static_assert(check_single_large_jordan_nonsym_slow<3>(), "Test large_jordan_slow_3 failed"); SUCCEED(); }
#endif
#endif
