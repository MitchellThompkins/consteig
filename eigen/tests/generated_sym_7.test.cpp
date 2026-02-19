#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_7) {
    static_assert(check_single_symmetric_fast<7>(), "Sym Fast 7 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_7) {
    static_assert(check_single_symmetric_slow<7>(), "Sym Slow 7 failed");
    SUCCEED();
}
#endif
