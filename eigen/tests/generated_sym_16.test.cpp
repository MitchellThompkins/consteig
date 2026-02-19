#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_16) {
    static_assert(check_single_symmetric_fast<16>(), "Sym Fast 16 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_16) {
    static_assert(check_single_symmetric_slow<16>(), "Sym Slow 16 failed");
    SUCCEED();
}
#endif
