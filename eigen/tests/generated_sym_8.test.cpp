#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_8) { static_assert(check_single_symmetric<8>(), "Sym 8 failed"); SUCCEED(); }
