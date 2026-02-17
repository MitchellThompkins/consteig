#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_11) { static_assert(check_single_nonsymmetric<11>(), "NonSym 11 failed"); SUCCEED(); }
