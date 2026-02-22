#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, nearly_reducible_10x10_24) { static_assert(check_single_nearly_reducible_nonsym_10x10<24>(), "Test nearly_reducible_10x10_24 failed"); SUCCEED(); }
#endif
