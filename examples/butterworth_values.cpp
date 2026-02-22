#include "butterworth_core.hpp"

// This variable is initialized at compile-time.
// objdump -s -j filter_data butterworth_values.cpp.o
// will show ONLY the raw bytes of the pre-calculated filter.
extern const butterworth::Constants g_butterworth_filter
    __attribute__((section(".filter_data"))) = butterworth::design();
