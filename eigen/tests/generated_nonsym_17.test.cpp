#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_17) { static_assert(check_single_nonsymmetric<17>(), "NonSym 17 failed"); SUCCEED(); }
