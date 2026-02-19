#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_25) {
    static_assert(check_single_symmetric_fast<25>(), "Sym Fast 25 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_25) {
    static_assert(check_single_symmetric_slow<25>(), "Sym Slow 25 failed");
    SUCCEED();
}
#endif
