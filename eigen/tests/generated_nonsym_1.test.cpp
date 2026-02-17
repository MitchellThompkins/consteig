#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_1) { static_assert(check_single_nonsymmetric<1>(), "NonSym 1 failed"); SUCCEED(); }
