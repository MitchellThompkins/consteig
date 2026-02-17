#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_23) { static_assert(check_single_nonsymmetric<23>(), "NonSym 23 failed"); SUCCEED(); }
