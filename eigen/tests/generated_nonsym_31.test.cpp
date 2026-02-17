#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_31) { static_assert(check_single_nonsymmetric<31>(), "NonSym 31 failed"); SUCCEED(); }
