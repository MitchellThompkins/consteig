#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_0) { static_assert(check_single_nonsymmetric<0>(), "NonSym 0 failed"); SUCCEED(); }
