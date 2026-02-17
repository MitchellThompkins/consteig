#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_15) { static_assert(check_single_nonsymmetric<15>(), "NonSym 15 failed"); SUCCEED(); }
