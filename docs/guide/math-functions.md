---
title: Math Functions
---

# Math Functions

consteig provides a set of constexpr math functions that work without the
standard library. All functions are in the `consteig` namespace and are included
via `consteig.hpp`.

## Basic Functions

| Function    | Description                                  |
|-------------|----------------------------------------------|
| `abs(x)`    | Absolute value (real or complex)             |
| `sqrt(x)`   | Square root (real); compile error if `x < 0` |
| `csqrt(x)`  | Complex square root                          |
| `sgn(x)`    | Sign: returns -1, 0, or 1                    |
| `exp(x)`    | Exponential e^x                              |
| `pow(x, n)` | x raised to integer power n                  |

consteig's built-in implementations were inspired by GCEM [^1]. GCEM is also
available as an optional drop-in math backend — see [Optional gcem Backend](#optional-gcem-backend) below.

## Trigonometric Functions

| Function  | Description        |
|-----------|--------------------|
| `sin(x)`  | Sine               |
| `cos(x)`  | Cosine             |
| `tan(x)`  | Tangent            |

Trigonometric functions use Taylor series expansion with `CONSTEIG_TRIG_MAX_ITER` iterations (default 20, sufficient for `double` precision).

## Complex Arithmetic

`Complex<T>` supports standard arithmetic operators and the following functions:

| Function  | Description                       |
|-----------|-----------------------------------|
| `abs(z)`  | Modulus: `sqrt(re^2 + im^2)`      |
| `conj(z)` | Conjugate: negates imaginary part |

## Utility Functions

| Function                    | Description                                         |
|-----------------------------|-----------------------------------------------------|
| `is_float<T>()`             | Returns `true` for `float`, `double`, `long double` |
| `equalWithin(a, b, thresh)` | Absolute tolerance comparison: `abs(a-b) < thresh`  |
| `epsilon<T>()`              | Machine epsilon for type `T`                        |

## Constants

| Macro      | Value                        |
|------------|------------------------------|
| `E_CONST`  | Euler's number e (50 digits) |
| `PI_CONST` | π (50 digits)                |

## Usage Example

```cpp
#include <consteig/consteig.hpp>

static constexpr double angle = PI_CONST / 4.0;         // 45 degrees
static constexpr double s = consteig::sin(angle);       // ~ 0.7071
static constexpr double c = consteig::cos(angle);       // ~ 0.7071

static constexpr consteig::Complex<double> z{3.0, 4.0};
static constexpr double mag = consteig::abs(z);         // 5.0
static constexpr auto zbar  = consteig::conj(z);        // 3 - 4i
```

## Negative Square Root

Attempting to take the square root of a negative number at compile time produces
a build error that identifies the problem:

```
error: 'force_compile_time_error_for_negative_sqrt' is not a constant expression
```

At runtime, a NaN (or -1 for integer types) is returned as a safe poison value.

## Optional gcem Backend

consteig ships with gcem vendored under `include/consteig/optional_dependencies/gcem/`.
Defining `CONSTEIG_USE_GCEM` before including consteig (or passing
`-DCONSTEIG_USE_GCEM=ON` to CMake) replaces the built-in math implementations
with gcem's equivalents. All `consteig::` math names remain the same — the
switch is transparent to user code.

### Type Traits Mode

gcem needs type traits to operate. Three modes are available, selected by
defining a macro before any consteig header is included. The mode must be
**uniform across all translation units** — mixing modes violates the One
Definition Rule and produces silent undefined behaviour.

| Macro | Effect |
|-------|--------|
| _(none, default)_ | Freestanding — uses compiler builtins. No stdlib required. |
| `CONSTEIG_GCEM_USE_STDLIB` | Uses `<limits>` and `<type_traits>` from the hosted stdlib. |
| `CONSTEIG_GCEM_USE_CUSTOM_TRAITS` | You supply trait definitions in `namespace gcem` (see below). |

`CONSTEIG_GCEM_USE_STDLIB` and `CONSTEIG_GCEM_USE_CUSTOM_TRAITS` are mutually exclusive.

### Custom Traits

When `CONSTEIG_GCEM_USE_CUSTOM_TRAITS` is defined, define the following in
`namespace gcem` **before** including any consteig header:

```cpp
namespace gcem {
    template<typename T> T&& declval() noexcept;
    template<class T> struct gcem_limits;
    template<bool B, typename T=void> struct enable_if;
    template<typename T> struct is_integral;
    template<typename T> struct is_signed;
    template<bool B, typename T, typename F> struct conditional;
    template<typename... T> struct common_type;
}
#define CONSTEIG_GCEM_USE_CUSTOM_TRAITS
#include <consteig/consteig.hpp>
```

### CMake

```cmake
# Default freestanding mode
target_link_libraries(my_target PRIVATE consteig::consteig)
cmake -DCONSTEIG_USE_GCEM=ON ..

# Hosted stdlib traits
cmake -DCONSTEIG_USE_GCEM=ON -DCONSTEIG_GCEM_USE_STDLIB=ON ..

# Custom traits
cmake -DCONSTEIG_USE_GCEM=ON -DCONSTEIG_GCEM_USE_CUSTOM_TRAITS=ON ..
```

[^1]: O'Hara, Keith. GCE-Math (Generalized Constant Expression Math) [GCEM](https://github.com/kthohr/gcem)
