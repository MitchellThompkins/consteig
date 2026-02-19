#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_12) {
    static_assert(check_single_symmetric_fast<12>(), "Sym Fast 12 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_12) {
    static_assert(check_single_symmetric_slow<12>(), "Sym Slow 12 failed");
    SUCCEED();
}
#endif
