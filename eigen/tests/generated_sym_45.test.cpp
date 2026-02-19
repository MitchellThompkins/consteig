#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_45) {
    static_assert(check_single_symmetric_fast<45>(), "Sym Fast 45 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_45) {
    static_assert(check_single_symmetric_slow<45>(), "Sym Slow 45 failed");
    SUCCEED();
}
#endif
