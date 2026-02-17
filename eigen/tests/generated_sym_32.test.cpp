#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_32) { static_assert(check_single_symmetric<32>(), "Sym 32 failed"); SUCCEED(); }
