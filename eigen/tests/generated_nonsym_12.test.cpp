#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_12) { static_assert(check_single_nonsymmetric<12>(), "NonSym 12 failed"); SUCCEED(); }
