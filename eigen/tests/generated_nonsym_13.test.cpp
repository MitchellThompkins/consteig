#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_13) { static_assert(check_single_nonsymmetric<13>(), "NonSym 13 failed"); SUCCEED(); }
