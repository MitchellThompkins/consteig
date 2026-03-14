#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include "functions/abs.hpp"
#include "functions/sqrt.hpp"
#include "functions/utilities.hpp"

namespace consteig
{

template <typename T> struct Complex
{
    T real;
    T imag;

    constexpr Complex(T r = 0, T i = 0) : real(r), imag(i)
    {
    }

    constexpr Complex operator+(const Complex &other) const
    {
        return {real + other.real, imag + other.imag};
    }

    constexpr Complex &operator+=(const Complex &other)
    {
        real += other.real;
        imag += other.imag;
        return *this;
    }

    constexpr Complex operator-(const Complex &other) const
    {
        return {real - other.real, imag - other.imag};
    }

    constexpr Complex operator*(const Complex &other) const
    {
        return {real * other.real - imag * other.imag,
                real * other.imag + imag * other.real};
    }

    constexpr Complex operator/(const Complex &other) const
    {
        T denom = other.real * other.real + other.imag * other.imag;
        return {(real * other.real + imag * other.imag) / denom,
                (imag * other.real - real * other.imag) / denom};
    }

    constexpr bool operator==(const Complex &other) const
    {
        return real == other.real && imag == other.imag;
    }

    constexpr bool operator!=(const Complex &other) const
    {
        return !(*this == other);
    }
};

template <typename T>
constexpr Complex<T> operator+(const T &scalar, const Complex<T> &c)
{
    return {scalar + c.real, c.imag};
}

template <typename T>
constexpr Complex<T> operator+(const Complex<T> &c, const T &scalar)
{
    return {c.real + scalar, c.imag};
}

template <typename T>
constexpr Complex<T> operator-(const T &scalar, const Complex<T> &c)
{
    return {scalar - c.real, -c.imag};
}

template <typename T>
constexpr Complex<T> operator-(const Complex<T> &c, const T &scalar)
{
    return {c.real - scalar, c.imag};
}

template <typename T>
constexpr Complex<T> operator*(const T &scalar, const Complex<T> &c)
{
    return {scalar * c.real, scalar * c.imag};
}

template <typename T>
constexpr Complex<T> operator*(const Complex<T> &c, const T &scalar)
{
    return scalar * c;
}

template <typename T> constexpr T abs(const Complex<T> &c)
{
    return consteig::sqrt(c.real * c.real + c.imag * c.imag);
}

template <typename T> constexpr Complex<T> conj(const Complex<T> &c)
{
    return {c.real, -c.imag};
}

template <typename T> constexpr Complex<T> csqrt(const T x)
{
    if (x < static_cast<T>(0))
    {
        if constexpr (is_float<T>())
        {
            return {static_cast<T>(0), consteig::sqrt(consteig::abs(x))};
        }
        else
        {
            // Cast to unsigned long long and negate to safely calculate the absolute
            // value of x, even if x is INT_MIN or INT64_MIN, avoiding overflow.
            return {static_cast<T>(0),
                    static_cast<T>(consteig::sqrt(-static_cast<unsigned long long>(x)))};
        }
    }
    else
    {
        return {consteig::sqrt(x), static_cast<T>(0)};
    }
}

} // namespace consteig

#endif
