#include "generated_test_helpers.hpp"
TEST(generated_tests, sym_42) { static_assert(check_single_symmetric<42>(), "Sym 42 failed"); SUCCEED(); }
