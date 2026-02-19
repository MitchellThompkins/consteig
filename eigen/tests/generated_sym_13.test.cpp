#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_13) {
    static_assert(check_single_symmetric_fast<13>(), "Sym Fast 13 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_13) {
    static_assert(check_single_symmetric_slow<13>(), "Sym Slow 13 failed");
    SUCCEED();
}
#endif
