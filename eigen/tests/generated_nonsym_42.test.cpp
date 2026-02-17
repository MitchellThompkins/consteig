#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_42) { static_assert(check_single_nonsymmetric<42>(), "NonSym 42 failed"); SUCCEED(); }
