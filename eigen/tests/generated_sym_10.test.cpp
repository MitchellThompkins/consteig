#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_10) { static_assert(check_single_symmetric<10>(), "Sym 10 failed"); SUCCEED(); }
