#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_28) {
    static_assert(check_single_symmetric_fast<28>(), "Sym Fast 28 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_28) {
    static_assert(check_single_symmetric_slow<28>(), "Sym Slow 28 failed");
    SUCCEED();
}
#endif
