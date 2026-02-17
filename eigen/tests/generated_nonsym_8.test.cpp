#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_8) { static_assert(check_single_nonsymmetric<8>(), "NonSym 8 failed"); SUCCEED(); }
