#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, large_jordan_10x10_38) { static_assert(check_single_large_jordan_nonsym_10x10<38>(), "Test large_jordan_10x10_38 failed"); SUCCEED(); }
#endif
