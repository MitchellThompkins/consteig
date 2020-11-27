#include <iostream>

#include "stddef.h"
#include "../constmat.hpp"


//using namespace constmat;

int main()
{
    static constexpr int x {3};

    static constexpr constmat::Matrix<int, 1, x> mat0
    {{{{5, -4, 2}}}};
    //static constexpr Matrix<int, 1, x> mat1
    //{{{{-1, 2 , 3}}}};
    //static constexpr Matrix<int, 1, x> mat2
    //{{{{-2, 1, 0}}}};

    //Matrix<int,x,x> mat {};
    //mat.setRow(mat0,0);
    //mat.setRow(mat1,1);
    //mat.setRow(mat2,2);

    //constexpr Matrix<int, x, x> answer
    //{{{{5, -4, 2}, {-1, 2 , 3}, {-2, 1, 0}}}};

    //for(int i {0}; i<x; i++)
    //{
    //    for(int j {0}; j<x; j++)
    //    {
    //        std::cout << mat(i,j) << " ";
    //    }
    //    std::cout << "\n";
    //}

    //std::cout << "\n";
    //for(int i {0}; i<x; i++)
    //{
    //    for(int j {0}; j<x; j++)
    //    {
    //        std::cout << answer(i,j) << " ";
    //    }
    //    std::cout << "\n";
    //}

    //Matrix<float, 2, 2> matTest
    //{{{{1.0F, -7.0F}, {-2.0F, 3.0F}}}};

    //static constexpr Matrix<float, x, x> three
    //{{{{5, -4, 2}, {-1, 2 , 3}, {-2, 1, 0}}}};

    //static constexpr float n3 {det(three)};

    //std::cout << n3 << "\n";

    //Matrix<int, x, x> colTest
    //{{{{5, -4, 2}, {-1, 2 , 3}, {-2, 1, 0}}}};

    //auto colExtract = colTest.col<1,2>(0);

    //for(int i {0}; i<2; i++)
    //{
    //    std::cout << colExtract(i,0) << " ";
    //}

    //static constexpr Matrix<int, 1, 3> rowTest1
    //{{{{99, -7, 8}}}};

    //static constexpr Matrix<int, 1, 2> rowTest2
    //{{{{-1, 257}}}};

    //std::cout << "\n\nBefore:\n";
    //for(int i {0}; i<x; i++)
    //{
    //    for(int j {0}; j<x; j++)
    //    {
    //        std::cout << colTest(i,j) << " ";
    //    }
    //    std::cout << "\n";
    //}

    ////colTest.setRow( rowTest1, 0 );

    //std::cout << "\n\nAfter 1:\n";
    //for(int i {0}; i<x; i++)
    //{
    //    for(int j {0}; j<x; j++)
    //    {
    //        std::cout << colTest(i,j) << " ";
    //    }
    //    std::cout << "\n";
    //}

    //colTest.setRow<1,2>( rowTest2, 1 );

    //std::cout << "\n\nAfter 2:\n";
    //for(int i {0}; i<x; i++)
    //{
    //    for(int j {0}; j<x; j++)
    //    {
    //        std::cout << colTest(i,j) << " ";
    //    }
    //    std::cout << "\n";
    //}
}
