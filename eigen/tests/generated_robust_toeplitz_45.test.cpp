#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, toeplitz_10x10_45) { static_assert(check_single_toeplitz_nonsym_10x10<45>(), "Test toeplitz_10x10_45 failed"); SUCCEED(); }
#endif
