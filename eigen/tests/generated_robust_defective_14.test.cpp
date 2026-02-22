#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, defective_10x10_14) { static_assert(check_single_defective_nonsym_10x10<14>(), "Test defective_10x10_14 failed"); SUCCEED(); }
#endif
