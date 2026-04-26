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
 * Freestanding standin for <utility>.
 * Provides declval for use in unevaluated contexts (needed by common_type).
 */

#if defined(GCEM_TRAITS_CUSTOM)

    // Nothing to do - user provides definitions in namespace gcem.

#elif defined(GCEM_TRAITS_BUILTIN)

namespace gcem
{

    // declval - for use in unevaluated contexts only; intentionally undefined
    // Standardized in <utility>, but GCC defines it in <type_traits> which <utility> includes.
    // https://github.com/gcc-mirror/gcc/blob/c39e4949694f15b4bd7f7b4de2769d853688508e/libstdc%2B%2B-v3/include/std/type_traits#L1076
    template<typename T>
    T&& declval() noexcept;

}  // namespace gcem

#else  // default (stdlib)

#include <utility>

namespace gcem
{
    using std::declval;
}  // namespace gcem

#endif
