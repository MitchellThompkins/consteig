/*################################################################################
  ##
  ##   Copyright (C) 2016-2026 Keith O'Hara
  ##
  ##   This file is part of the GCE-Math C++ library.
  ##
  ##   Licensed under the Apache License, Version 2.0 (the "License");
  ##   you may not use this file except in compliance with the License.
  ##   You may obtain a copy of the License at
  ##
  ##       http://www.apache.org/licenses/LICENSE-2.0
  ##
  ##   Unless required by applicable law or agreed to in writing, software
  ##   distributed under the License is distributed on an "AS IS" BASIS,
  ##   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  ##   See the License for the specific language governing permissions and
  ##   limitations under the License.
  ##
  ################################################################################*/

/*
 * Type traits and numeric limits dispatch.
 *
 * Three modes, selected by define before including gcem.hpp:
 *
 *   (default)            Use <limits> and <type_traits> from the hosted stdlib.
 *   GCEM_TRAITS_BUILTIN  Self-contained implementation; no external includes.
 *                        Uses IEEE 754 arithmetic for infinity/NaN.
 *                        Suitable for freestanding / bare-metal targets
 *                        using GCC or Clang.
 *   GCEM_TRAITS_CUSTOM   Skip all definitions. The user must define the following
 *                        in namespace gcem before including gcem.hpp:
 *                          template<class T> struct gcem_limits;
 *                          template<bool B, typename T=void> struct enable_if;
 *                          template<typename T> struct is_integral;
 *                          template<typename T> struct is_signed;
 *                          template<bool B, typename T, typename F> struct conditional;
 *                          template<typename... T> struct common_type;
 */

#if defined(GCEM_TRAITS_CUSTOM)

    // Nothing to do - user provides definitions in namespace gcem.

#elif defined(GCEM_TRAITS_BUILTIN)

namespace gcem
{

    // declval - for use in unevaluated contexts only; intentionally undefined
    template<typename T>
    T&& declval() noexcept;

    // enable_if
    template<bool B, typename T = void>
    struct enable_if {};

    template<typename T>
    struct enable_if<true, T> { using type = T; };

    // is_integral
    template<typename T> struct is_integral           { static constexpr bool value = false; };
    template<> struct is_integral<bool>               { static constexpr bool value = true;  };
    template<> struct is_integral<char>               { static constexpr bool value = true;  };
    template<> struct is_integral<signed char>        { static constexpr bool value = true;  };
    template<> struct is_integral<unsigned char>      { static constexpr bool value = true;  };
    template<> struct is_integral<short>              { static constexpr bool value = true;  };
    template<> struct is_integral<unsigned short>     { static constexpr bool value = true;  };
    template<> struct is_integral<int>                { static constexpr bool value = true;  };
    template<> struct is_integral<unsigned int>       { static constexpr bool value = true;  };
    template<> struct is_integral<long>               { static constexpr bool value = true;  };
    template<> struct is_integral<unsigned long>      { static constexpr bool value = true;  };
    template<> struct is_integral<long long>          { static constexpr bool value = true;  };
    template<> struct is_integral<unsigned long long> { static constexpr bool value = true;  };

    template<typename T> struct is_integral<const T>          : is_integral<T> {};
    template<typename T> struct is_integral<volatile T>       : is_integral<T> {};
    template<typename T> struct is_integral<const volatile T> : is_integral<T> {};

    // is_signed - T(-1) < T(0) works correctly for all arithmetic types in C++11
    template<typename T>
    struct is_signed { static constexpr bool value = T(-1) < T(0); };

    // conditional
    template<bool B, typename T, typename F>
    struct conditional { using type = F; };

    template<typename T, typename F>
    struct conditional<true, T, F> { using type = T; };

    // remove_reference / remove_cv / decay - needed to strip T&& from ternary results
    template<typename T> struct remove_reference      { using type = T; };
    template<typename T> struct remove_reference<T&>  { using type = T; };
    template<typename T> struct remove_reference<T&&> { using type = T; };

    template<typename T> struct remove_cv                   { using type = T; };
    template<typename T> struct remove_cv<const T>          { using type = T; };
    template<typename T> struct remove_cv<volatile T>       { using type = T; };
    template<typename T> struct remove_cv<const volatile T> { using type = T; };

    template<typename T>
    struct decay {
        using type = typename remove_cv<typename remove_reference<T>::type>::type;
    };

    // common_type - variadic, peels one type at a time via ternary decay
    // decay<> mirrors what std::common_type does: strips the && that declval introduces.
    template<typename... T> struct common_type;

    template<typename T>
    struct common_type<T> { using type = T; };

    template<typename T1, typename T2>
    struct common_type<T1, T2> {
        using type = typename decay<decltype(true ? declval<T1>() : declval<T2>())>::type;
    };

    template<typename T1, typename T2, typename... Rest>
    struct common_type<T1, T2, Rest...> {
        using type = typename common_type<
            typename decay<decltype(true ? declval<T1>() : declval<T2>())>::type, Rest...>::type;
    };

    // gcem_limits - floating-point specializations
    //
    // infinity() and quiet_NaN() use __builtin_* intrinsics, tested via __has_builtin.
    // min() and max() use compiler-predefined macros (__FLT_MIN__, __FLT_MAX__, etc.)
    // which expand to plain numeric literals - no function calls, no headers required.
    //
    // If your compiler lacks these builtins, use GCEM_TRAITS_CUSTOM instead.

    template<typename T>
    struct gcem_limits;

// GCC < 10 defines __has_builtin but it is not usable as a preprocessor operator.
// Provide a safe fallback so the check below works on all supported compilers.
#ifndef __has_builtin
  #define __has_builtin(x) 0
  #define GCEM_UNDEF_HAS_BUILTIN
#endif

#if __has_builtin(__builtin_huge_valf) || defined(__GNUC__) || defined(__clang__)

    template<>
    struct gcem_limits<float> {
        static constexpr float infinity()  noexcept { return __builtin_huge_valf(); }
        static constexpr float quiet_NaN() noexcept { return __builtin_nanf("");    }
        static constexpr float min()       noexcept { return __FLT_MIN__;           }
        static constexpr float max()       noexcept { return __FLT_MAX__;           }
        static constexpr float epsilon()   noexcept { return __FLT_EPSILON__;       }
    };

    template<>
    struct gcem_limits<double> {
        static constexpr double infinity()  noexcept { return __builtin_huge_val(); }
        static constexpr double quiet_NaN() noexcept { return __builtin_nan("");    }
        static constexpr double min()       noexcept { return __DBL_MIN__;          }
        static constexpr double max()       noexcept { return __DBL_MAX__;          }
        static constexpr double epsilon()   noexcept { return __DBL_EPSILON__;      }
    };

    template<>
    struct gcem_limits<long double> {
        static constexpr long double infinity()  noexcept { return __builtin_huge_vall(); }
        static constexpr long double quiet_NaN() noexcept { return __builtin_nanl("");    }
        static constexpr long double min()       noexcept { return __LDBL_MIN__;          }
        static constexpr long double max()       noexcept { return __LDBL_MAX__;          }
        static constexpr long double epsilon()   noexcept { return __LDBL_EPSILON__;      }
    };

#else
    #error "GCEM_TRAITS_BUILTIN: compiler does not support __builtin_huge_valf. " \
           "Use GCEM_TRAITS_CUSTOM and provide your own gcem_limits specializations."
#endif

#ifdef GCEM_UNDEF_HAS_BUILTIN
  #undef __has_builtin
  #undef GCEM_UNDEF_HAS_BUILTIN
#endif

    // Integer specializations (min/max used by pow_integral.hpp)
    // Uses unsigned arithmetic to compute signed limits without UB:
    //   max() = all bits set except sign bit = (unsigned(-1) >> 1) cast to signed
    //   min() = -max() - 1  (two's complement, universally true in practice)

    template<>
    struct gcem_limits<int> {
        static constexpr int max() noexcept { return static_cast<int>(static_cast<unsigned int>(-1) >> 1); }
        static constexpr int min() noexcept { return -max() - 1; }
    };

    template<>
    struct gcem_limits<long> {
        static constexpr long max() noexcept { return static_cast<long>(static_cast<unsigned long>(-1) >> 1); }
        static constexpr long min() noexcept { return -max() - 1L; }
    };

    template<>
    struct gcem_limits<long long> {
        static constexpr long long max() noexcept { return static_cast<long long>(static_cast<unsigned long long>(-1) >> 1); }
        static constexpr long long min() noexcept { return -max() - 1LL; }
    };

    template<>
    struct gcem_limits<unsigned int> {
        static constexpr unsigned int min() noexcept { return 0U; }
        static constexpr unsigned int max() noexcept { return static_cast<unsigned int>(-1); }
    };

    template<>
    struct gcem_limits<unsigned long> {
        static constexpr unsigned long min() noexcept { return 0UL; }
        static constexpr unsigned long max() noexcept { return static_cast<unsigned long>(-1); }
    };

    template<>
    struct gcem_limits<unsigned long long> {
        static constexpr unsigned long long min() noexcept { return 0ULL; }
        static constexpr unsigned long long max() noexcept { return static_cast<unsigned long long>(-1); }
    };

}  // namespace gcem

#else  // GCEM_TRAITS_STDLIB (default)

#include <limits>
#include <type_traits>

namespace gcem
{

    using std::enable_if;
    using std::is_integral;
    using std::is_signed;
    using std::conditional;
    using std::common_type;

    template<class T>
    struct gcem_limits : std::numeric_limits<T> {};

}  // namespace gcem

#endif
