#ifndef CONSTEIG_TYPES_HPP
#define CONSTEIG_TYPES_HPP

namespace consteig
{
// Define Size type to avoid dependency on <cstddef>
using Size = decltype(sizeof(0));
} // namespace consteig

#endif
