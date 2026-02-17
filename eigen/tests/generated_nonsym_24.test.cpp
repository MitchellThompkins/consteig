#include "generated_test_helpers.hpp"
TEST(generated_tests, nonsym_24) { static_assert(check_single_nonsymmetric<24>(), "NonSym 24 failed"); SUCCEED(); }
