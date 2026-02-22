#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, nearly_reducible_10x10_6)
{
    static_assert(check_single_nearly_reducible_nonsym_10x10<6>(),
                  "Test nearly_reducible_10x10_6 failed");
    SUCCEED();
}
#endif
