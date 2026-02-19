#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_42) {
    static_assert(check_single_symmetric_fast<42>(), "Sym Fast 42 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_42) {
    static_assert(check_single_symmetric_slow<42>(), "Sym Slow 42 failed");
    SUCCEED();
}
#endif
