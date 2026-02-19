#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "../consteig_types.hpp"

namespace consteig {

template <typename T, Size N>
class Array {
   public:
    constexpr T& operator[](const Size i) { return _data[i]; }
    constexpr const T& operator[](const Size i) const { return _data[i]; }

    T* data() { return &_data[0]; }

    static Size constexpr size() { return N; }

    constexpr T* begin() { return &_data[0]; }
    constexpr const T* begin() const { return &_data[0]; }

    constexpr T* end() { return &_data[N]; }
    constexpr const T* end() const { return &_data[N]; }

    T _data[N];
};

template <Size I, typename T, Size N>
constexpr T& get(Array<T, N>& a) {
    static_assert(I < N, "I must be less than N");
    return a[I];
}

template <Size I, typename T, Size N>
constexpr const T& get(const Array<T, N>& a) {
    static_assert(I < N, "I must be less than N");
    return a[I];
}

}  // namespace consteig

#endif  // ARRAY_HPP
