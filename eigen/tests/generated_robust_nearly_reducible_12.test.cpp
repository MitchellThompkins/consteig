#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, nearly_reducible_10x10_12)
{
    static_assert(check_single_nearly_reducible_nonsym_10x10<12>(),
                  "Test nearly_reducible_10x10_12 failed");
    SUCCEED();
}
#endif
