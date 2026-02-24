#include "generated_test_helpers.hpp"
TEST(generated_tests, clustered_8x8_42) { static_assert(check_single_clustered_nonsym_8x8<42>(), "Test clustered_8x8_42 failed"); SUCCEED(); }
