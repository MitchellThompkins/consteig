#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, nearly_defective_8x8_13) { static_assert(check_single_nearly_defective_nonsym_8x8<13>(), "Test nearly_defective_8x8_13 failed"); SUCCEED(); }
#endif
