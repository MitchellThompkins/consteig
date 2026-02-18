#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_46) { static_assert(check_single_symmetric_fast<46>(), "Sym Fast 46 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_46) { static_assert(check_single_symmetric_slow<46>(), "Sym Slow 46 failed"); SUCCEED(); }
#endif
