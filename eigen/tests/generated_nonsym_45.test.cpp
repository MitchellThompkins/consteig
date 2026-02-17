#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_45) { static_assert(check_single_nonsymmetric<45>(), "NonSym 45 failed"); SUCCEED(); }
