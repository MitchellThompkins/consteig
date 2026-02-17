#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_6) { static_assert(check_single_nonsymmetric<6>(), "NonSym 6 failed"); SUCCEED(); }
