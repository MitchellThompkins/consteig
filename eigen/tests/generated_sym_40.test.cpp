#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_40) {
    static_assert(check_single_symmetric_fast<40>(), "Sym Fast 40 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_40) {
    static_assert(check_single_symmetric_slow<40>(), "Sym Slow 40 failed");
    SUCCEED();
}
#endif
