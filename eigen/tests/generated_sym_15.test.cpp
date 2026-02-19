#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_15) {
    static_assert(check_single_symmetric_fast<15>(), "Sym Fast 15 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_15) {
    static_assert(check_single_symmetric_slow<15>(), "Sym Slow 15 failed");
    SUCCEED();
}
#endif
