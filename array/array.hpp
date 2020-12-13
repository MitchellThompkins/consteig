#ifndef ARRAY_HPP
#define ARRAY_HPP

namespace consteig
{

template <typename T, size_t N>
class Array
{
public:

    constexpr T& operator[](const size_t i)
    {
        return _data[i];
    }
    constexpr const T& operator[](const size_t i) const
    {
        return _data[i];
    }

    T* data()
    {
        return &_data[0];
    }

    static size_t constexpr size()
    {
        return N;
    }

    constexpr T* begin()
    {
        return &_data[0];
    }
    constexpr const T* begin() const
    {
        return &_data[0];
    }

    constexpr T* end()
    {
        return &_data[N];
    }
    constexpr const T* end() const
    {
        return &_data[N];
    }

    T _data[N];
};

template <size_t I, typename T, size_t N>
constexpr T& get(Array<T, N>& a)
{
    static_assert(I < N, "I must be less than N");
    return a[I];
}

template <size_t I, typename T, size_t N>
constexpr const T& get(const Array<T, N>& a)
{
    static_assert(I < N, "I must be less than N");
    return a[I];
}

} //end namespace

#endif // ARRAY_HPP
