#include "matrix.hpp"
#include <iostream>

int main()
{
    static constexpr int x {3};

    static constexpr Matrix<int, 1, x> mat0
    {{{{5, -4, 2}}}};
    static constexpr Matrix<int, 1, x> mat1
    {{{{-1, 2 , 3}}}};
    static constexpr Matrix<int, 1, x> mat2
    {{{{-2, 1, 0}}}};

    Matrix<int,x,x> mat {};
    mat.setRow(mat0,0);
    mat.setRow(mat1,1);
    mat.setRow(mat2,2);

    static constexpr Matrix<int, x, x> answer
    {{{{5, -4, 2}, {-1, 2 , 3}, {-2, 1, 0}}}};

    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            std::cout << mat(i,j) << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            std::cout << answer(i,j) << " ";
        }
        std::cout << "\n";
    }
}
