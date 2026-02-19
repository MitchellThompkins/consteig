#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_3) {
    static_assert(check_single_symmetric_fast<3>(), "Sym Fast 3 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_3) {
    static_assert(check_single_symmetric_slow<3>(), "Sym Slow 3 failed");
    SUCCEED();
}
#endif
