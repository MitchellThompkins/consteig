#ifndef CONSTEIG_OPTIONS_HPP
#define CONSTEIG_OPTIONS_HPP

#ifndef CONSTEIG_MAX_ITER
#define CONSTEIG_MAX_ITER 500
#endif

#ifndef CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE
#define CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE 1e-6
#endif

// Uncomment to force all internal constexpr eigenvalue calculations to use long
// double. This improves numerical stability for large/pathological matrices but
// is very resource intensive for the compiler. #define CONSTEIG_USE_LONG_DOUBLE

#endif
