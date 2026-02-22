#include "butterworth_core.hpp"

// This variable is initialized at compile-time.
// objdump -s -j .rodata butterworth_values.cpp.o 
// will show the raw bytes of the pre-calculated filter.
extern const butterworth::Constants g_butterworth_filter = butterworth::design();
