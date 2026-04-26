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
 * Freestanding standin for <limits>.
 * Provides gcem_limits, a minimal numeric_limits replacement.
 */

#if defined(GCEM_TRAITS_CUSTOM)

    // Nothing to do - user provides definitions in namespace gcem.

#elif defined(GCEM_TRAITS_BUILTIN)

namespace gcem
{

    // gcem_limits - mirrors std::numeric_limits for the members gcem uses.
    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L391
    //
    // infinity() and quiet_NaN() use __builtin_* intrinsics, tested via __has_builtin.
    // min(), max(), and epsilon() use compiler-predefined macros (__FLT_MIN__, etc.)
    // which expand to plain numeric literals - no function calls, no headers required.
    //
    // If your compiler lacks these builtins, use GCEM_TRAITS_CUSTOM instead.

    template<typename T>
    struct gcem_limits;

// GCC < 10 defines __has_builtin but it is not usable as a preprocessor operator.
// Provide a safe fallback so the check below works on all supported compilers.
// Note: defining a __-prefixed name is technically reserved by the standard, but
// this is a widely used pattern in freestanding code with no portable alternative.
#ifndef __has_builtin
  #define __has_builtin(x) 0
  #define GCEM_UNDEF_HAS_BUILTIN
#endif

#if __has_builtin(__builtin_huge_valf) || defined(__GNUC__) || defined(__clang__)

    template<>
    struct gcem_limits<float> {
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1681
        static constexpr float min()       noexcept { return __FLT_MIN__;           }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1684
        static constexpr float max()       noexcept { return __FLT_MAX__;           }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1703
        static constexpr float epsilon()   noexcept { return __FLT_EPSILON__;       }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1722
        static constexpr float infinity()  noexcept { return __builtin_huge_valf(); }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1725
        static constexpr float quiet_NaN() noexcept { return __builtin_nanf("");    }
    };

    template<>
    struct gcem_limits<double> {
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1756
        static constexpr double min()       noexcept { return __DBL_MIN__;          }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1759
        static constexpr double max()       noexcept { return __DBL_MAX__;          }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1778
        static constexpr double epsilon()   noexcept { return __DBL_EPSILON__;      }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1797
        static constexpr double infinity()  noexcept { return __builtin_huge_val(); }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1800
        static constexpr double quiet_NaN() noexcept { return __builtin_nan("");    }
    };

    template<>
    struct gcem_limits<long double> {
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1831
        static constexpr long double min()       noexcept { return __LDBL_MIN__;          }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1834
        static constexpr long double max()       noexcept { return __LDBL_MAX__;          }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1853
        static constexpr long double epsilon()   noexcept { return __LDBL_EPSILON__;      }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1872
        static constexpr long double infinity()  noexcept { return __builtin_huge_vall(); }
        // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1875
        static constexpr long double quiet_NaN() noexcept { return __builtin_nanl("");    }
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
    // Uses unsigned arithmetic to compute signed limits without UB rather than
    // INT_MAX/INT_MIN macros (which would require <climits>). Equivalent values,
    // different implementation:
    //   max() = all bits set except sign bit = (unsigned(-1) >> 1) cast to signed
    //   min() = -max() - 1  (two's complement; mandated by C++20, de facto universal
    //                        pre-C++20 -- this library intentionally requires it)

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1067
    template<>
    struct gcem_limits<int> {
        static constexpr int max() noexcept { return static_cast<int>(static_cast<unsigned int>(-1) >> 1); }
        static constexpr int min() noexcept { return -max() - 1; }
    };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1206
    template<>
    struct gcem_limits<long> {
        static constexpr long max() noexcept { return static_cast<long>(static_cast<unsigned long>(-1) >> 1); }
        static constexpr long min() noexcept { return -max() - 1L; }
    };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1346
    template<>
    struct gcem_limits<long long> {
        static constexpr long long max() noexcept { return static_cast<long long>(static_cast<unsigned long long>(-1) >> 1); }
        static constexpr long long min() noexcept { return -max() - 1LL; }
    };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1134
    template<>
    struct gcem_limits<unsigned int> {
        static constexpr unsigned int min() noexcept { return 0U; }
        static constexpr unsigned int max() noexcept { return static_cast<unsigned int>(-1); }
    };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1273
    template<>
    struct gcem_limits<unsigned long> {
        static constexpr unsigned long min() noexcept { return 0UL; }
        static constexpr unsigned long max() noexcept { return static_cast<unsigned long>(-1); }
    };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1416
    template<>
    struct gcem_limits<unsigned long long> {
        static constexpr unsigned long long min() noexcept { return 0ULL; }
        static constexpr unsigned long long max() noexcept { return static_cast<unsigned long long>(-1); }
    };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L858
    template<>
    struct gcem_limits<bool> {
        static constexpr bool min() noexcept { return false; }
        static constexpr bool max() noexcept { return true; }
    };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L464
    // char signedness is implementation-defined; the ternary detects it at compile time
    // (mirrors __glibcxx_min/max(char) from libstdc++).
    template<>
    struct gcem_limits<char> {
        static constexpr char max() noexcept {
            return static_cast<char>(-1) < static_cast<char>(0)
                ? static_cast<char>(static_cast<unsigned char>(-1) >> 1)
                : static_cast<char>(static_cast<unsigned char>(-1));
        }
        static constexpr char min() noexcept {
            return static_cast<char>(-1) < static_cast<char>(0) ? -max() - 1 : 0;
        }
    };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L531
    template<>
    struct gcem_limits<signed char> {
        static constexpr signed char min() noexcept { return -__SCHAR_MAX__ - 1; }
        static constexpr signed char max() noexcept { return __SCHAR_MAX__; }
    };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L601
    template<>
    struct gcem_limits<unsigned char> {
        static constexpr unsigned char min() noexcept { return 0; }
        static constexpr unsigned char max() noexcept { return static_cast<unsigned char>(-1); }
    };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L932
    template<>
    struct gcem_limits<short> {
        static constexpr short min() noexcept { return -__SHRT_MAX__ - 1; }
        static constexpr short max() noexcept { return __SHRT_MAX__; }
    };

    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/limits#L1051
    template<>
    struct gcem_limits<unsigned short> {
        static constexpr unsigned short min() noexcept { return 0; }
        static constexpr unsigned short max() noexcept { return static_cast<unsigned short>(-1); }
    };

}  // namespace gcem

#else  // default (stdlib)

#include <limits>

namespace gcem
{

    template<class T>
    struct gcem_limits : std::numeric_limits<T> {};

}  // namespace gcem

#endif
