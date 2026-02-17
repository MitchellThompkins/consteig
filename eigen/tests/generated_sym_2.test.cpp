#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_2) { static_assert(check_single_symmetric<2>(), "Sym 2 failed"); SUCCEED(); }
