#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "../consteig_types.hpp"

namespace consteig
{

/// @brief Fixed-size array with compile-time length.
///
/// A freestanding alternative to `std::array` used as the internal storage
/// type for @ref Matrix. Most element-access operations are `constexpr`;
/// the `data()` overloads are non-`constexpr`.
///
/// @tparam T  Element type.
/// @tparam N  Number of elements (compile-time constant).
template <typename T, Size N> class Array
{
  public:
    /// @brief Access element at index `i` (mutable).
    constexpr T &operator[](const Size i)
    {
        return _data[i];
    }
    /// @brief Access element at index `i` (read-only).
    constexpr const T &operator[](const Size i) const
    {
        return _data[i];
    }

    /// @brief Raw pointer to the first element (non-constexpr).
    T *data()
    {
        return &_data[0];
    }
    /// @brief Raw pointer to the first element (read-only).
    const T *data() const
    {
        return &_data[0];
    }

    /// @brief Number of elements in the array.
    static Size constexpr size()
    {
        return N;
    }

    /// @brief Iterator to the first element (mutable).
    constexpr T *begin()
    {
        return &_data[0];
    }
    /// @brief Iterator to the first element (read-only).
    constexpr const T *begin() const
    {
        return &_data[0];
    }

    /// @brief One-past-the-end iterator (mutable).
    constexpr T *end()
    {
        return &_data[N];
    }
    /// @brief One-past-the-end iterator (read-only).
    constexpr const T *end() const
    {
        return &_data[N];
    }

    /// @brief Raw storage. Direct access is permitted but prefer `operator[]`.
    T _data[N];
};

/// @brief Compile-time indexed element access (mutable).
///
/// @tparam I  Compile-time index. Must satisfy `I < N`.
/// @tparam T  Element type.
/// @tparam N  Array size.
/// @param  a  Array to index into.
/// @return Reference to the element at position `I`.
template <Size I, typename T, Size N> constexpr T &get(Array<T, N> &a)
{
    static_assert(I < N, "I must be less than N");
    return a[I];
}

/// @brief Compile-time indexed element access (read-only).
///
/// @tparam I  Compile-time index. Must satisfy `I < N`.
/// @tparam T  Element type.
/// @tparam N  Array size.
/// @param  a  Array to index into.
/// @return Const reference to the element at position `I`.
template <Size I, typename T, Size N>
constexpr const T &get(const Array<T, N> &a)
{
    static_assert(I < N, "I must be less than N");
    return a[I];
}

} // namespace consteig

#endif // ARRAY_HPP
