#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nearly_defective_slow_8) { static_assert(check_single_nearly_defective_nonsym_slow<8>(), "Test nearly_defective_slow_8 failed"); SUCCEED(); }
#endif
#endif
