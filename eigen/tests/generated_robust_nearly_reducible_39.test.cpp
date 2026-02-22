#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, nearly_reducible_10x10_39)
{
    static_assert(check_single_nearly_reducible_nonsym_10x10<39>(),
                  "Test nearly_reducible_10x10_39 failed");
    SUCCEED();
}
#endif
