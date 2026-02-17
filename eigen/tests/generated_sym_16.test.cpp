#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_16) { static_assert(check_single_symmetric<16>(), "Sym 16 failed"); SUCCEED(); }
