#ifndef CONSTMATH_SQRT_HPP
#define CONSTMATH_SQRT_HPP

#include "utilities.hpp" // For is_float()

namespace consteig
{

// For floating point types
// https://www.geeksforgeeks.org/find-root-of-a-number-using-newtons-method/
template<typename T>
constexpr T sqrt(
        const T val,
        const T limit = 1e-7,
        const unsigned int countLimit = 511)
{

    // The closed guess will be stored in the root
    T root {static_cast<T>(0)};

    // For floating point
    if( is_float<T>() && val > static_cast<T>(0) )
    {
        T valCopy {val};

        unsigned int count {0};
        //TODO(mthompkins): Add iteration limit
        while(true && count < countLimit)
        {
            count++;
            // Calculate more closed x
            root = 0.5 * (valCopy + (val / valCopy));

            // Check for closeness
            if (abs(root - valCopy) < limit)
                break;

            // Update root
            valCopy = root;
        }
    }
    // For non-floating point
    else if(val > static_cast<T>(0))
    {
        // Base cases
        if (val == 0 || val == 1)
            return val;

        // Staring from 1, try all numbers until
        // i*i is greater than or equal to val.
        T i = 1, result = 1;
        while (result <= val)
        {
            i++;
            result = i * i;
        }
        root = i - 1;
    }
    // 0 or less than 0
    else
    {
        // Do nothing
    }

    return root;
}

} //end namespace

#endif
