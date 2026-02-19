#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_23) {
    static_assert(check_single_symmetric_fast<23>(), "Sym Fast 23 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_23) {
    static_assert(check_single_symmetric_slow<23>(), "Sym Slow 23 failed");
    SUCCEED();
}
#endif
