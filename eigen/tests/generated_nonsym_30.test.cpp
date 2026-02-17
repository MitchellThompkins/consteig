#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_30) { static_assert(check_single_nonsymmetric<30>(), "NonSym 30 failed"); SUCCEED(); }
