#include "qr_decomp.hpp"
#include "gcem.hpp"
#include <iostream>

int main()
{
    int y = 0;

    static constexpr int x {3};

    //static constexpr Matrix<float, x, x> mat
    //{{{ {1.0F, 1.0F, 0.0F}, {1.0F, 0.0F , 1.0F}, {0.0F, 1.0F, 1.0F} }}};

    Matrix<float, x, x> mat
    {{{ {1.0F, 6.0F, 62.0F}, {1.0F, 5.0F , 22.0F}, {-1.0F, 3.0F, 11.0F} }}};

    QrMatrix<float, x> qr {qrDecomp(mat)};

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

    //std::cout << "\n";
    //for(int i {0}; i<x; i++)
    //{
    //    for(int j {0}; j<x; j++)
    //    {
    //        std::cout << qAnswer(i,j) << " ";
    //    }
    //    std::cout << "\n";
    //}

    std::cout << "\nR Matrix:\n";
    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            std::cout << qr._r(i,j) << " ";
        }
        std::cout << "\n";
    }

    //std::cout << "\n";
    //for(int i {0}; i<x; i++)
    //{
    //    for(int j {0}; j<x; j++)
    //    {
    //        std::cout << rAnswer(i,j) << " ";
    //    }
    //    std::cout << "\n";
    //}

}
