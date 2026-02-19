#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_26) {
    static_assert(check_single_symmetric_fast<26>(), "Sym Fast 26 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_26) {
    static_assert(check_single_symmetric_slow<26>(), "Sym Slow 26 failed");
    SUCCEED();
}
#endif
