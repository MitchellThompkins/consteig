#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_12) { static_assert(check_single_symmetric<12>(), "Sym 12 failed"); SUCCEED(); }
