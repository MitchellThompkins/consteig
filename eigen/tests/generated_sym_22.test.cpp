#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_22) {
    static_assert(check_single_symmetric_fast<22>(), "Sym Fast 22 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_22) {
    static_assert(check_single_symmetric_slow<22>(), "Sym Slow 22 failed");
    SUCCEED();
}
#endif
