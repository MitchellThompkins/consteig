#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_7) { static_assert(check_single_nonsymmetric<7>(), "NonSym 7 failed"); SUCCEED(); }
