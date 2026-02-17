#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_32) { static_assert(check_single_nonsymmetric<32>(), "NonSym 32 failed"); SUCCEED(); }
