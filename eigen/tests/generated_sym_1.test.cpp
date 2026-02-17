#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_1) { static_assert(check_single_symmetric<1>(), "Sym 1 failed"); SUCCEED(); }
