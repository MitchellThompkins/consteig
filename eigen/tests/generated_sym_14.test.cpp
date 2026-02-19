#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_14) {
    static_assert(check_single_symmetric_fast<14>(), "Sym Fast 14 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_14) {
    static_assert(check_single_symmetric_slow<14>(), "Sym Slow 14 failed");
    SUCCEED();
}
#endif
