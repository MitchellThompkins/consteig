#ifndef CONSTEIG_TYPES_HPP
#define CONSTEIG_TYPES_HPP

namespace consteig
{
/// @brief Unsigned integer type used for all matrix dimensions and indices.
///
/// Equivalent to `std::size_t` but defined without including `<cstddef>`,
/// preserving the library's freestanding property. All @ref Matrix template
/// parameters `R` and `C` are of this type.
using Size = decltype(sizeof(0));
} // namespace consteig

#endif
