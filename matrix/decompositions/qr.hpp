#ifndef QR_DECOMP_HPP
#define QR_DECOMP_HPP

#include "../matrix.hpp"
#include "../operations.hpp"
#include "../../math/functions/abs.hpp"
#include "../../math/functions/sqrt.hpp"
#include "../../math/functions/utilities.hpp"

namespace consteig
{

template<typename T, Size S>
struct QRMatrix
{
    Matrix<T,S,S> _q;
    Matrix<T,S,S> _r;
};

// Modified Gram-Schmidt QR Decomposition
template<typename T, Size R, Size C>
constexpr QRMatrix<T, R> qr( const Matrix<T,R,C> a )
{
    static_assert( R==C, "QR decomposition must be a square matrix");

    Matrix<T,R,R> q{}; 
    Matrix<T,R,R> r{};
    
    // Copy a to v (working matrix)
    Matrix<T,R,R> v = a;

    for(Size i = 0; i < R; ++i)
    {
        // r(i,i) = norm(v_i)
        T n = normE(v.col(i));
        r(i,i) = n;

        if(consteig::abs(n) > static_cast<T>(1e-15))
        {
            // q_i = v_i / r(i,i)
            T inv_n = static_cast<T>(1) / n;
            Matrix<T,R,1> q_col = inv_n * v.col(i);
            q.setCol(q_col, i);
        }

        for(Size j = i + 1; j < R; ++j)
        {
            // r(i,j) = q_i . v_j
            T d = dot(transpose(q.col(i)), transpose(v.col(j)));
            r(i,j) = d;
            
            // v_j = v_j - r(i,j) * q_i
            Matrix<T,R,1> v_col_new = v.col(j) - (d * q.col(i));
            v.setCol(v_col_new, j);
        }
    }

    QRMatrix<T, R> res;
    res._q = q;
    res._r = r;
    return res;
}

} //end namespace
#endif
