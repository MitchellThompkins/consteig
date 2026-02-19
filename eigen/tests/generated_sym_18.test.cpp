#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_18) {
    static_assert(check_single_symmetric_fast<18>(), "Sym Fast 18 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_18) {
    static_assert(check_single_symmetric_slow<18>(), "Sym Slow 18 failed");
    SUCCEED();
}
#endif
