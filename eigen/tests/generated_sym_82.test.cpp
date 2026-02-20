#include "generated_test_helpers.hpp"
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_slow_32) {
    static_assert(check_single_random_sym_slow<32>(), "Test random_slow_32 failed");
    SUCCEED();
}
#endif
