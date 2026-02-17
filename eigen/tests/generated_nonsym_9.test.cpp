#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_9) { static_assert(check_single_nonsymmetric<9>(), "NonSym 9 failed"); SUCCEED(); }
