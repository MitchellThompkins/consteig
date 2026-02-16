#ifndef HESSENBERG_HPP
#define HESSENBERG_HPP

#include "householder.hpp"

#include "../matrix.hpp"
#include "../operations.hpp"

namespace consteig
{

///////////// TYPES /////////////
template<typename T, size_t S>
struct PHMatrix
{
    Matrix<T,S,S> _p;
    Matrix<T,S,S> _h;

    constexpr void operator=(const PHMatrix<T, S> &rhs)
    {
        for( size_t i{0}; i<S; i++ )
            for( size_t j{0}; j<S; j++ )
            {
                (*this._p)(i,j) = rhs._p(i,j);
                (*this._h)(i,j) = rhs._h(i,j);
            }
    }
};

///////////// FUNCTION DECLARATIONS /////////////
template<typename T, size_t R, size_t C, size_t L=R>
constexpr PHMatrix<T,R> hess(Matrix<T,R,C> a);


///////////// IMPLEMENTATIONS /////////////

template<typename T, size_t R, size_t C, size_t L>
constexpr PHMatrix<T,R> hess(Matrix<T,R,C> a)
{
    static_assert( is_float<T>(), "hess expects floating point");
    static_assert( R==C, "Hessenberg reduction expects a square matrix");

    if constexpr (L <= 2) {
        PHMatrix<T,R> result;
        // _p should be Identity or just used as accumulator? 
        // In the original base case: ._p = {0}, ._h = a.
        // Wait, {0} for Matrix initializes to zero.
        // The recursive step does `pRtn = (houseSize>3) ? p*out._p : p`.
        // If base case returns 0 for P, then P*0 = 0?
        // Actually, if L=3, it calls hess<...2>. Returns p={0}.
        // Then (3>3) is false. pRtn = p. Correct.
        // So base case P is irrelevant if logic handles it, or should be I?
        // Original code returned {0}.
        // Let's stick to original behavior but explicit.
        result._p = Matrix<T,R,R>{}; // Zero
        result._h = a;
        return result;
    } else {
        constexpr size_t size {R};
        constexpr size_t houseSize {L};
        constexpr size_t end {R-1};

        Matrix<T,L,L> subA
        {
            a.template sub<
                R-houseSize,
                R-houseSize,
                end,
                end>()
        };
        Matrix<T,L,L> m {house(subA)};

        Matrix<T,size,size> p {eye<T,R>()};
        p.template setSub<
            R-houseSize+1,
            R-houseSize+1,
            end,
            end>(m.template sub<1,1,houseSize-1,houseSize-1>());

        PHMatrix<T,R> out = hess<T,R,R,houseSize-1>(p*a*p);

        Matrix<T,size,size> pRtn = (houseSize > 3) ? p*out._p : p;

        return {
            ._p = pRtn,
            ._h = out._h
        };
    }
};

} //end namespace
#endif
