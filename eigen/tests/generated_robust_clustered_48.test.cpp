#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, clustered_10x10_48) { static_assert(check_single_clustered_nonsym_10x10<48>(), "Test clustered_10x10_48 failed"); SUCCEED(); }
#endif
