#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_34) {
    static_assert(check_single_symmetric_fast<34>(), "Sym Fast 34 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_34) {
    static_assert(check_single_symmetric_slow<34>(), "Sym Slow 34 failed");
    SUCCEED();
}
#endif
