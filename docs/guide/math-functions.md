title: Math Functions

# Math Functions

consteig provides a set of constexpr math functions that work without the standard library. All functions are in the `consteig` namespace and are included via `consteig.hpp`.

## Basic Functions

| Function    | Description                                  |
|-------------|----------------------------------------------|
| `abs(x)`    | Absolute value (real or complex)             |
| `sqrt(x)`   | Square root (real); compile error if `x < 0` |
| `csqrt(x)`  | Complex square root                          |
| `sgn(x)`    | Sign: returns -1, 0, or 1                    |
| `exp(x)`    | Exponential e^x                              |
| `pow(x, n)` | x raised to integer power n                  |

## Trigonometric Functions

| Function  | Description        |
|-----------|--------------------|
| `sin(x)`  | Sine               |
| `cos(x)`  | Cosine             |
| `tan(x)`  | Tangent            |
| `asin(x)` | Arcsine            |
| `acos(x)` | Arccosine          |
| `atan(x)` | Arctangent         |
| `sinh(x)` | Hyperbolic sine    |
| `cosh(x)` | Hyperbolic cosine  |
| `tanh(x)` | Hyperbolic tangent |

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
#include "consteig.hpp"

static constexpr double angle = PI_CONST / 4.0;         // 45 degrees
static constexpr double s = consteig::sin(angle);       // ≈ 0.7071
static constexpr double c = consteig::cos(angle);       // ≈ 0.7071

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
