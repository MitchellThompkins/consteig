#include "qr_decomp.hpp"
#include "gcem.hpp"
#include <iostream>

int main()
{
    int y = 0;

    static constexpr int x {3};

    Matrix<float, x, x> mat
    {{{ {1.0F, 1.0F, 0.0F}, {1.0F, 0.0F , 1.0F}, {0.0F, 1.0F, 1.0F} }}};

    QrMatrix<float, x> qr {qrDecomp(mat)};

    static constexpr Matrix<float, x, x> qAnswer
    {{{
        {1.0F/gcem::sqrt(2),  1.0F/gcem::sqrt(6), -1.0F/gcem::sqrt(3)},
        {1.0F/gcem::sqrt(2), -1.0F/gcem::sqrt(6),  1.0F/gcem::sqrt(3)},
        {0.0F,                2.0F/gcem::sqrt(6),  1.0F/gcem::sqrt(3)}
    }}};

    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            std::cout << qr._q(i,j) << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            std::cout << qAnswer(i,j) << " ";
        }
        std::cout << "\n";
    }

}
