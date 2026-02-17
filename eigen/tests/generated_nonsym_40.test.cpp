#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_40) { static_assert(check_single_nonsymmetric<40>(), "NonSym 40 failed"); SUCCEED(); }
