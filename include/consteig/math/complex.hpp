#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include "../consteig_options.hpp"

#include "functions/utilities.hpp"

namespace consteig
{

/// @brief Constexpr complex number type.
///
/// A lightweight complex number with no dependency on `<complex>`.
/// All arithmetic operators and utility functions are `constexpr`, making
/// it suitable for compile-time computation. Returned by @ref eigenvalues and
/// @ref eigenvectors for complex eigenvalue pairs.
///
/// @tparam T  Floating-point scalar type for the real and imaginary parts.
///
/// @code
/// constexpr consteig::Complex<double> z{3.0, 4.0}; // 3 + 4i
/// constexpr double magnitude = consteig::abs(z);    // 5.0
/// constexpr auto zc = consteig::conj(z);            // 3 - 4i
/// @endcode
template <typename T> struct Complex
{
    T real; ///< Real part.
    T imag; ///< Imaginary part.

    /// @brief Construct from real and imaginary parts (default: 0 + 0i).
    constexpr Complex(T r = 0, T i = 0) : real(r), imag(i)
    {
    }

    /// @brief Complex addition.
    constexpr Complex operator+(const Complex &other) const
    {
        return {real + other.real, imag + other.imag};
    }

    /// @brief Complex addition in-place.
    constexpr Complex &operator+=(const Complex &other)
    {
        real += other.real;
        imag += other.imag;
        return *this;
    }

    /// @brief Complex subtraction.
    constexpr Complex operator-(const Complex &other) const
    {
        return {real - other.real, imag - other.imag};
    }

    /// @brief Complex multiplication: (a+bi)(c+di) = (ac-bd) + (ad+bc)i.
    constexpr Complex operator*(const Complex &other) const
    {
        return {real * other.real - imag * other.imag,
                real * other.imag + imag * other.real};
    }

    /// @brief Complex division: divides by `|other|^2`.
    constexpr Complex operator/(const Complex &other) const
    {
        T denom = other.real * other.real + other.imag * other.imag;
        return {(real * other.real + imag * other.imag) / denom,
                (imag * other.real - real * other.imag) / denom};
    }

    /// @brief Exact equality (no tolerance). Prefer @ref equalWithin for
    /// floats.
    constexpr bool operator==(const Complex &other) const
    {
        return real == other.real && imag == other.imag;
    }

    /// @brief Inequality.
    constexpr bool operator!=(const Complex &other) const
    {
        return !(*this == other);
    }
};

/// @brief Scalar + complex addition.
/// @tparam T  Scalar and complex element type.
template <typename T>
constexpr Complex<T> operator+(const T &scalar, const Complex<T> &c)
{
    return {scalar + c.real, c.imag};
}

/// @brief Complex + scalar addition.
/// @tparam T  Scalar and complex element type.
template <typename T>
constexpr Complex<T> operator+(const Complex<T> &c, const T &scalar)
{
    return {c.real + scalar, c.imag};
}

/// @brief Scalar - complex subtraction.
/// @tparam T  Scalar and complex element type.
template <typename T>
constexpr Complex<T> operator-(const T &scalar, const Complex<T> &c)
{
    return {scalar - c.real, -c.imag};
}

/// @brief Complex - scalar subtraction.
/// @tparam T  Scalar and complex element type.
template <typename T>
constexpr Complex<T> operator-(const Complex<T> &c, const T &scalar)
{
    return {c.real - scalar, c.imag};
}

/// @brief Scalar * complex multiplication.
/// @tparam T  Scalar and complex element type.
template <typename T>
constexpr Complex<T> operator*(const T &scalar, const Complex<T> &c)
{
    return {scalar * c.real, scalar * c.imag};
}

/// @brief Complex * scalar multiplication.
/// @tparam T  Scalar and complex element type.
template <typename T>
constexpr Complex<T> operator*(const Complex<T> &c, const T &scalar)
{
    return scalar * c;
}

/// @brief Absolute value (modulus) of a complex number: `sqrt(re^2 + im^2)`.
/// @tparam T  Floating-point element type.
/// @param  c  Input complex number.
/// @return Non-negative real magnitude.
template <typename T> constexpr T abs(const Complex<T> &c)
{
    return sqrt(c.real * c.real + c.imag * c.imag);
}

/// @brief Exponential of a complex number using Euler's formula.
///
/// exp(x + iy) = exp(x) * (cos(y) + i*sin(y))
///
/// @tparam T  Floating-point element type.
/// @param  z  Input complex number.
template <typename T> constexpr Complex<T> exp(const Complex<T> &z) noexcept
{
    const T ex = exp(z.real);
    return {ex * cos(z.imag), ex * sin(z.imag)};
}

/// @brief Complex conjugate: negates the imaginary part.
/// @tparam T  Element type.
/// @param  c  Input complex number.
/// @return `c.real - c.imag * i`.
template <typename T> constexpr Complex<T> conj(const Complex<T> &c)
{
    return {c.real, -c.imag};
}

} // namespace consteig

#endif
