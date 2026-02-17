#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_20) { static_assert(check_single_symmetric<20>(), "Sym 20 failed"); SUCCEED(); }
