#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_3) { static_assert(check_single_nonsymmetric<3>(), "NonSym 3 failed"); SUCCEED(); }
