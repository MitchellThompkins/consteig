#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_20) {
    static_assert(check_single_symmetric_fast<20>(), "Sym Fast 20 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_20) {
    static_assert(check_single_symmetric_slow<20>(), "Sym Slow 20 failed");
    SUCCEED();
}
#endif
