#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_8) {
    static_assert(check_single_symmetric_fast<8>(), "Sym Fast 8 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_8) {
    static_assert(check_single_symmetric_slow<8>(), "Sym Slow 8 failed");
    SUCCEED();
}
#endif
