#ifndef CONSTEIG_OPTIONS_HPP
#define CONSTEIG_OPTIONS_HPP

/// @defgroup config Configuration Macros
/// @brief Compile-time options for tuning solver behavior and precision.
///
/// All macros are guarded by `#ifndef` so they can be overridden by defining
/// them before including `consteig.hpp`, or by passing `-DMACRO=value` to
/// the compiler. The defaults are well-tested; change them only if you have
/// a specific need (e.g., very large matrices or pathological inputs).
/// @{

/// @def CONSTEIG_MAX_ITER
/// @brief Maximum QR iterations per eigenvalue block.
///
/// The solver runs at most `CONSTEIG_MAX_ITER * S` total iterations for an
/// `S x S` matrix. Increasing this may help difficult matrices converge at
/// the cost of longer compile times and a higher risk of hitting compiler
/// constexpr step limits.
#ifndef CONSTEIG_MAX_ITER
#define CONSTEIG_MAX_ITER 500
#endif

/// @def CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE
/// @brief Symmetry routing threshold for eigensolver selection.
///
/// If `isSymmetric(thresh)` returns `true` for the input matrix, the
/// faster single-shift QR algorithm (`eig_shifted_qr`) is used instead of
/// the double-shift variant. Tighten this value to force the double-shift
/// solver on nearly-symmetric matrices; loosen it to accept more matrices
/// as symmetric.
#ifndef CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE
#define CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE 1e-6
#endif

/// @def CONSTEIG_BALANCE_CONVERGENCE_THRESHOLD
/// @brief Stopping criterion for the matrix balancing step.
///
/// A row/column scaling is applied only if it reduces the sum of the row and
/// column norms by more than this factor. The default `0.95` is taken from
/// Algorithm 2 of James, Langou & Lowery (2014). Increasing toward `1.0`
/// runs more balancing iterations; decreasing it stops earlier.
#ifndef CONSTEIG_BALANCE_CONVERGENCE_THRESHOLD
#define CONSTEIG_BALANCE_CONVERGENCE_THRESHOLD 0.95
#endif

/// @def E_CONST
/// @brief Euler's number *e* to 50 significant digits.
#ifndef E_CONST
#define E_CONST 2.71828182845904523536028747135266249775724709369995
#endif

/// @def PI_CONST
/// @brief The constant π to 50 significant digits.
#ifndef PI_CONST
#define PI_CONST 3.14159265358979323846264338327950288419716939937510
#endif

/// @def CONSTEIG_TRIG_MAX_ITER
/// @brief Maximum Taylor series iterations for trigonometric functions.
///
/// 14 iterations suffice for `double` precision (worst case x=π:
/// π^29/29! ≈ 3e-17 < machine epsilon). The default of 20 gives a
/// comfortable margin. Increase only if using `long double` with extreme
/// argument values.
#ifndef CONSTEIG_TRIG_MAX_ITER
// 14 iterations suffices for double precision (worst case x=pi: pi^29/29! ~
// 3e-17 < machine epsilon). 20 gives a comfortable margin.
#define CONSTEIG_TRIG_MAX_ITER 20
#endif

/// @def CONSTEIG_USE_LONG_DOUBLE
/// @brief Force all internal eigenvalue calculations to use `long double`.
///
/// When defined, the internal scalar type (`InternalScalar`) is promoted to
/// `long double`, improving numerical stability for large or pathological
/// matrices. This is very resource-intensive for the compiler and will
/// significantly increase compile times. Combine with `-mlong-double-80`
/// and `-mfpmath=387` on x86 for 80-bit extended precision.
// Uncomment to force all internal constexpr eigenvalue calculations to use long
// double. This improves numerical stability for large/pathological matrices but
// is very resource intensive for the compiler.
// #define CONSTEIG_USE_LONG_DOUBLE

/// @def CONSTEIG_USE_GCEM
/// @brief Use gcem math functions instead of consteig's built-in
/// implementations.
///
/// When defined, `consteig::sqrt()`, `consteig::abs()`, `consteig::exp()`,
/// `consteig::sin()`, `consteig::cos()`, `consteig::tan()`, `consteig::pow()`,
/// and `consteig::sgn()` delegate to their `gcem::` counterparts.
/// Requires gcem to be vendored into
/// `include/consteig/optional_dependencies/gcem/` (run
/// `scripts/vendor_gcem.sh`).
///
/// By default, gcem is configured to use compiler builtins only (freestanding).
/// Define `CONSTEIG_GCEM_USE_STDLIB` to allow gcem to use stdlib headers.
// #define CONSTEIG_USE_GCEM

/// @def CONSTEIG_GCEM_USE_STDLIB
/// @brief Allow gcem to use `<limits>` and `<type_traits>` from the standard
/// library.
///
/// Only meaningful when `CONSTEIG_USE_GCEM` is defined. When NOT defined (the
/// default), gcem operates in freestanding mode using compiler builtins
/// (`GCEM_TRAITS_BUILTIN`), preserving consteig's no-stdlib property.
/// Define this macro to allow gcem to use stdlib type traits, which may
/// improve compatibility on hosted platforms.
///
/// Mutually exclusive with `CONSTEIG_GCEM_USE_CUSTOM_TRAITS`.
// #define CONSTEIG_GCEM_USE_STDLIB

/// @def CONSTEIG_GCEM_USE_CUSTOM_TRAITS
/// @brief Supply your own gcem type trait definitions.
///
/// Only meaningful when `CONSTEIG_USE_GCEM` is defined. When defined, gcem
/// skips all built-in trait definitions and expects the following to be
/// provided in `namespace gcem` before any consteig header is included:
///
/// @code
/// namespace gcem {
///     template<typename T> T&& declval() noexcept;
///     template<class T> struct gcem_limits;
///     template<bool B, typename T=void> struct enable_if;
///     template<typename T> struct is_integral;
///     template<typename T> struct is_signed;
///     template<bool B, typename T, typename F> struct conditional;
///     template<typename... T> struct common_type;
/// }
/// @endcode
///
/// @warning The trait mode must be **uniform across all translation units**.
/// Mixing modes violates the One Definition Rule and produces silent undefined
/// behaviour.
///
/// Mutually exclusive with `CONSTEIG_GCEM_USE_STDLIB`.
// #define CONSTEIG_GCEM_USE_CUSTOM_TRAITS

/// @}  // defgroup config

#endif
