#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, nearly_defective_10x10_34) { static_assert(check_single_nearly_defective_nonsym_10x10<34>(), "Test nearly_defective_10x10_34 failed"); SUCCEED(); }
#endif
