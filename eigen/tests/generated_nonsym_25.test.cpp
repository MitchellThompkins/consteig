#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_25) { static_assert(check_single_nonsymmetric<25>(), "NonSym 25 failed"); SUCCEED(); }
