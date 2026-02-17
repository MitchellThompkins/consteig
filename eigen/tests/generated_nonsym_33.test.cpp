#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_33) { static_assert(check_single_nonsymmetric<33>(), "NonSym 33 failed"); SUCCEED(); }
