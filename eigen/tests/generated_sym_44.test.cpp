#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_44) {
    static_assert(check_single_symmetric_fast<44>(), "Sym Fast 44 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_44) {
    static_assert(check_single_symmetric_slow<44>(), "Sym Slow 44 failed");
    SUCCEED();
}
#endif
