#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, nearly_reducible_8x8_15) { static_assert(check_single_nearly_reducible_nonsym_8x8<15>(), "Test nearly_reducible_8x8_15 failed"); SUCCEED(); }
#endif
