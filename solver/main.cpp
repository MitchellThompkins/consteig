#include "qr_decomp.hpp"
#include "solver.hpp"
#include "gcem.hpp"
#include <iostream>

#include "test_tools.hpp"

template<typename T, size_t R, size_t C>
static inline bool checkEigenValues(
        const Matrix<T,R,C> a,
        const Matrix<T,R,1> lambda )
{
    //det(A-lambda*I)
    bool equal {true};
    auto identity {diagional<T,R>(static_cast<T>(1))};

    for(int i {0}; i<R; i++)
    {
        auto detResult {det( a - (lambda(i,0)*identity))};
        auto compareResult {compareFloat( det( a - (lambda(i,0)*identity)), static_cast<T>(0), 0.0009F )};

        equal &= compareFloat( det( a - (lambda(i,0)*identity)), static_cast<T>(0), 0.0009F );
    }

    return equal;
}

int main()
{
    int y = 0;

    static constexpr int x {3};

    ///*static constexpr*/ Matrix<float, x, x> mat
    //{{{ {1.0F, 1.0F, 0.0F}, {1.0F, 0.0F , 1.0F}, {0.0F, 1.0F, 1.0F} }}};

    Matrix<float, x, x> mat
    {{{ {1.0F, 6.0F, 62.0F}, {1.0F, 5.0F , 22.0F}, {-1.0F, 3.0F, 11.0F} }}};

    std::cout << "\nA Matrix:\n";
    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            //std::cout << qr._q(i,j) << " ";
            std::cout << mat(i,j) << " ";
        }
        std::cout << "\n";
    }


    /*static constexpr*/ QrMatrix<float, x> qr {qrDecomp(mat)};

    static constexpr Matrix<float, x, x> qAnswer
    {{{
        {1.0F/gcem::sqrt(2.0F),  1.0F/gcem::sqrt(6.0F), -1.0F/gcem::sqrt(3.0F)},
        {1.0F/gcem::sqrt(2.0F), -1.0F/gcem::sqrt(6.0F),  1.0F/gcem::sqrt(3.0F)},
        {0.0F,                   2.0F/gcem::sqrt(6.0F),  1.0F/gcem::sqrt(3.0F)}
    }}};

    static constexpr Matrix<float, x, x> rAnswer
    {{{
        {2.0F/gcem::sqrt(2.0F),  1.0F/gcem::sqrt(2.0F),  1.0F/gcem::sqrt(2.0F)},
        {0.0F,                   3.0F/gcem::sqrt(6.0F),  1.0F/gcem::sqrt(6.0F)},
        {0.0F,                   0.0F,                   2.0F/gcem::sqrt(3.0F)}
    }}};

    std::cout << "\nQ Matrix:\n";
    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            std::cout << qr._q(i,j) << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nR Matrix:\n";
    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            std::cout << qr._r(i,j) << " ";
        }
        std::cout << "\n";
    }

    static constexpr Matrix<float, x, x> test {qAnswer*transpose(qAnswer)};
    Matrix<float,x,x> identity {diagional<float,x>(1.0F)};
    Matrix<float,x,x> iden2 {diagional<float,x>(static_cast<float>(1U))};


    static constexpr Matrix<float,4,4> matlab
    {{{
        { -4.4529e-01,  4.9063e+00, -8.7871e-01,  6.3036e+00},
        { -6.3941e+00,  1.3354e+01,  1.6668e+00,  1.1945e+01},
        {  3.6842e+00, -6.6617e+00, -6.0021e-02, -7.0043e+00},
        {  3.1209e+00, -5.2052e+00, -1.4130e+00, -2.8484e+00}
    }}};

    /*static constexpr*/ UtMatrix<float, 4> eigenSolverTest {eigensolver(matlab)};
    static constexpr auto eigenValueTest {eigenvalues(matlab)};

    std::cout << "\nU Matrix:\n";
    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            //std::cout << qr._q(i,j) << " ";
            std::cout << eigenSolverTest._u(i,j) << " ";
        }
        std::cout << "\n";
    }

    //std::cout << "\n";
    //for(int i {0}; i<x; i++)
    //{
    //    for(int j {0}; j<x; j++)
    //    {
    //        std::cout << qAnswer(i,j) << " ";
    //    }
    //    std::cout << "\n";
    //}

    std::cout << "\nT Matrix:\n";
    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            std::cout << eigenSolverTest._t(i,j) << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            std::cout << rAnswer(i,j) << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nEigen Values:\n";
    for(size_t i {0}; i<4; i++)
    {
        std::cout << eigenValueTest(i,0) << "\n";
    }
    std::cout << "\n";

    std::cout << "\nEigen Value Test:\n";
    static constexpr Matrix<float,4,4> identityEigen {diagional<float,4>(1.0F)};
    for(int i {0}; i<4; i++)
    {
        std::cout << det(matlab - (eigenValueTest(i,0)*identityEigen)) << "\n";
    }

    bool equal = checkEigenValues<float,4,4>(matlab,eigenValueTest);
    std::cout << equal << "\n";

}
