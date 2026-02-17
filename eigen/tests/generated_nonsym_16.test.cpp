#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_16) { static_assert(check_single_nonsymmetric<16>(), "NonSym 16 failed"); SUCCEED(); }
