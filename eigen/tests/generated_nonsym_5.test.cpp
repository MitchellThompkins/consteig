#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_5) { static_assert(check_single_nonsymmetric<5>(), "NonSym 5 failed"); SUCCEED(); }
