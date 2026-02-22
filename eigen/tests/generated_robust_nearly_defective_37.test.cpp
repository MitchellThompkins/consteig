#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, nearly_defective_10x10_37) { static_assert(check_single_nearly_defective_nonsym_10x10<37>(), "Test nearly_defective_10x10_37 failed"); SUCCEED(); }
#endif
