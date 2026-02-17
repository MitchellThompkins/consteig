#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_21) { static_assert(check_single_nonsymmetric<21>(), "NonSym 21 failed"); SUCCEED(); }
