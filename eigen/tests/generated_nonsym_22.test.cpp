#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_22) { static_assert(check_single_nonsymmetric<22>(), "NonSym 22 failed"); SUCCEED(); }
