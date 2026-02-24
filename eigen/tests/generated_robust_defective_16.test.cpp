#include "generated_test_helpers.hpp"
TEST(generated_tests, defective_8x8_16) { static_assert(check_single_defective_nonsym_8x8<16>(), "Test defective_8x8_16 failed"); SUCCEED(); }
