#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_3) { static_assert(check_single_symmetric<3>(), "Sym 3 failed"); SUCCEED(); }
