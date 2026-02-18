#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_43) { static_assert(check_single_symmetric_fast<43>(), "Sym Fast 43 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_43) { static_assert(check_single_symmetric_slow<43>(), "Sym Slow 43 failed"); SUCCEED(); }
#endif
