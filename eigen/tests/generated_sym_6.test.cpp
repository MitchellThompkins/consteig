#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_6) {
    static_assert(check_single_symmetric_fast<6>(), "Sym Fast 6 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_6) {
    static_assert(check_single_symmetric_slow<6>(), "Sym Slow 6 failed");
    SUCCEED();
}
#endif
