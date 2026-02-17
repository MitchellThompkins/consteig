#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_4) { static_assert(check_single_nonsymmetric<4>(), "NonSym 4 failed"); SUCCEED(); }
