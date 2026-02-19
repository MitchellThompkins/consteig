#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_47) {
    static_assert(check_single_symmetric_fast<47>(), "Sym Fast 47 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_47) {
    static_assert(check_single_symmetric_slow<47>(), "Sym Slow 47 failed");
    SUCCEED();
}
#endif
