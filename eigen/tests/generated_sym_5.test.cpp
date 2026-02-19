#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_5) {
    static_assert(check_single_symmetric_fast<5>(), "Sym Fast 5 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_5) {
    static_assert(check_single_symmetric_slow<5>(), "Sym Slow 5 failed");
    SUCCEED();
}
#endif
