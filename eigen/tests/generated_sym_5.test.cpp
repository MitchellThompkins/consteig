#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_5) { static_assert(check_single_symmetric<5>(), "Sym 5 failed"); SUCCEED(); }
