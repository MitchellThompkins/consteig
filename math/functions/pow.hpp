#ifndef CONSTMATH_POW_HPP
#define CONSTMATH_POW_HPP

namespace consteig
{

template<typename T>
constexpr T pow(const T x, const unsigned int n)
{
    //https://docs.microsoft.com/en-us/cpp/cpp/constexpr-cpp?view=msvc-160
    //TODO(mthompkins): Need to find a better error handling method than just
    //returning 0 for negavie numbers. Should really handle negative numbers
    return
        n<0 ? static_cast<T>(0) :
        n == 0 ? 1 :
        n % 2 == 0 ? pow(x * x, n / 2) :
        pow(x * x, (n - 1) / 2) * x;
}

} //end namespace

#endif
