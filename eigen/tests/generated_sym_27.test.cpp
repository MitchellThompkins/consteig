#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_27) {
    static_assert(check_single_symmetric_fast<27>(), "Sym Fast 27 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_27) {
    static_assert(check_single_symmetric_slow<27>(), "Sym Slow 27 failed");
    SUCCEED();
}
#endif
