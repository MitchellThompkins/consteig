#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_23) { static_assert(check_single_symmetric<23>(), "Sym 23 failed"); SUCCEED(); }
