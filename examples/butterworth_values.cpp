#include "butterworth_core.hpp"

#ifdef __APPLE__
#define FILTER_DATA_SECTION "__DATA,filter_data"
#else
#define FILTER_DATA_SECTION ".filter_data"
#endif

// This variable is initialized at compile-time.
// objdump -s -j filter_data butterworth_values.cpp.o
// will show ONLY the raw bytes of the pre-calculated filter.
extern const butterworth::Constants g_butterworth_filter
    __attribute__((section(FILTER_DATA_SECTION))) = butterworth::design();
