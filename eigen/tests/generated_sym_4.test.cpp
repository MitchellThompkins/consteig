#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_4) { static_assert(check_single_symmetric<4>(), "Sym 4 failed"); SUCCEED(); }
