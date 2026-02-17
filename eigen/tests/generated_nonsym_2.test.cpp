#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_2) { static_assert(check_single_nonsymmetric<2>(), "NonSym 2 failed"); SUCCEED(); }
