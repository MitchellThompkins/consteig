#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_10) { static_assert(check_single_nonsymmetric<10>(), "NonSym 10 failed"); SUCCEED(); }
