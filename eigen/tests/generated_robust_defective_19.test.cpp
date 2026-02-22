#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, defective_10x10_19) { static_assert(check_single_defective_nonsym_10x10<19>(), "Test defective_10x10_19 failed"); SUCCEED(); }
#endif
