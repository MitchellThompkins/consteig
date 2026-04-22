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
 * Type traits dispatch.
 *
 * Three modes, selected by define before including gcem.hpp:
 *
 *   (default)            Use <utility>, <limits>, and <type_traits> from the hosted stdlib.
 *   GCEM_TRAITS_BUILTIN  Self-contained implementation; no external includes.
 *                        Suitable for freestanding / bare-metal targets
 *                        using GCC or Clang.
 *   GCEM_TRAITS_CUSTOM   Skip all definitions. The user must define the following
 *                        in namespace gcem before including gcem.hpp:
 *                          template<typename T> T&& declval() noexcept;
 *                          template<class T> struct gcem_limits;
 *                          template<bool B, typename T=void> struct enable_if;
 *                          template<typename T> struct is_integral;
 *                          template<typename T> struct is_signed;
 *                          template<bool B, typename T, typename F> struct conditional;
 *                          template<typename... T> struct common_type;
 *
 * ODR WARNING: The mode MUST be uniform across every translation unit in a
 * binary. In BUILTIN mode gcem::enable_if, gcem::is_integral, etc. are
 * independent struct definitions; in default mode they are aliases for the
 * std:: types. Mixing modes across TUs violates the One Definition Rule and
 * produces silent undefined behaviour. Enforce a consistent setting via a
 * project-wide compiler flag (e.g. -DGCEM_TRAITS_BUILTIN) rather than
 * per-file defines.
 */

#include "gcem_utility.hpp"  // required for declval, used by common_type in all three modes

#if defined(GCEM_TRAITS_CUSTOM)

    // Nothing to do - user provides definitions in namespace gcem.

#elif defined(GCEM_TRAITS_BUILTIN)

namespace gcem
{

    // enable_if
    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/type_traits#L136
    template<bool B, typename T = void>
    struct enable_if {};

    template<typename T>
    struct enable_if<true, T> { using type = T; };

    // is_integral
    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/type_traits#L540
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

    // is_signed - T(-1) < T(0) works correctly for all arithmetic types in C++11.
    // Diverges from std::is_signed for non-arithmetic types: std::is_signed gates
    // this expression behind is_arithmetic and returns false for class types,
    // whereas this will fail to compile if T(-1) is not a valid expression.
    // Safe here because every call site constrains T to integral types.
    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/type_traits#L1055
    template<typename T>
    struct is_signed { static constexpr bool value = T(-1) < T(0); };

    // conditional
    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/type_traits#L2565
    template<bool B, typename T, typename F>
    struct conditional { using type = F; };

    template<typename T, typename F>
    struct conditional<true, T, F> { using type = T; };

    // remove_reference / remove_cv / decay - needed to strip T&& from ternary results
    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/type_traits#L1859
    template<typename T> struct remove_reference      { using type = T; };
    template<typename T> struct remove_reference<T&>  { using type = T; };
    template<typename T> struct remove_reference<T&&> { using type = T; };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/type_traits#L1793
    template<typename T> struct remove_cv                   { using type = T; };
    template<typename T> struct remove_cv<const T>          { using type = T; };
    template<typename T> struct remove_cv<volatile T>       { using type = T; };
    template<typename T> struct remove_cv<const volatile T> { using type = T; };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/type_traits#L2491
    template<typename T>
    struct decay {
        using type = typename remove_cv<typename remove_reference<T>::type>::type;
    };

    // common_type - variadic, peels one type at a time via ternary decay
    // decay<> mirrors what std::common_type does: strips the && that declval introduces.
    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/type_traits#L2575
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

}  // namespace gcem

#else  // default (stdlib)

#include <type_traits>

namespace gcem
{

    using std::enable_if;
    using std::is_integral;
    using std::is_signed;
    using std::conditional;
    using std::common_type;

}  // namespace gcem

#endif
