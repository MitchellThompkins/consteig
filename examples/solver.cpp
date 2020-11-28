#include "example_helper.hpp"
#include "../consteig.hpp"

//template<typename T, size_t R, size_t C>
//static inline bool checkEigenValues(
//        const Matrix<T,R,C> a,
//        const Matrix<T,R,1> lambda )
//{
//    //det(A-lambda*I)
//    bool equal {true};
//    auto identity {diagional<T,R>(static_cast<T>(1))};
//
//    for(int i {0}; i<R; i++)
//    {
//        auto detResult {det( a - (lambda(i,0)*identity))};
//        auto compareResult {compareFloat( det( a - (lambda(i,0)*identity)), static_cast<T>(0), 0.0009F )};
//
//        equal &= compareFloat( det( a - (lambda(i,0)*identity)), static_cast<T>(0), 0.0009F );
//    }
//
//    return equal;
//}

int main()
{
}

// T = [ -4.4529e-01  4.9063e+00 -8.7871e-01  6.3036e+00; -6.3941e+00  1.3354e+01  1.6668e+00  1.1945e+01; 3.6842e+00 -6.6617e+00 -6.0021e-02 -7.0043e+00; 3.1209e+00 -5.2052e+00 -1.4130e+00 -2.8484e+00; ];
