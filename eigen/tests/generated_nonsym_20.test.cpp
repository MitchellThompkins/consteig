#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_20) { static_assert(check_single_nonsymmetric<20>(), "NonSym 20 failed"); SUCCEED(); }
