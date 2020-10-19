#include "matrix.hpp"
#include <iostream>

int main()
{
    static constexpr int r {3};
    static constexpr int c {2};

    static constexpr Matrix<int, r, c> mat1 {{{{3, 4}, {5, 6}, {3, 9}}}};
    static constexpr Matrix<int, c, r> mat2 {{{{1, -3, 7}, {2, 5, 9}}}};

    static constexpr Matrix<int, r, r> answer1
    {{{{11, 11, 57}, {17, 15, 89}, {21, 36, 102}}}};

    static constexpr Matrix<int, r, r> mult1 { mat1*mat2 };

    const bool n = (answer1 == mult1);

    std::cout << n << "\n";

    Matrix<int, r, c> mat3 {{{{3, 4}, {5, 6}, {3, 9}}}};
    Matrix<int, c, r> mat4 {{{{1, -3, 7}, {2, 5, 9}}}};

    auto test = mat1*mat2;

    std::cout << test(0,1) << "\n";

    static constexpr Matrix<int, r, c> mat7 {{{{3, 4}, {5, 6}, {3, 9}}}};
    auto multi = transpose(mat7*transpose(mat7));

    Matrix<int, c, c> mat5;
    mat5(0,0) = 99;
    mat5(0,1) = 7;
    mat5(1,0) = 47;
    mat5(1,1) = 3;

    Matrix<int, c, c> mat6;
    mat6(0,0) = 99;
    mat6(0,1) = 7;
    mat6(1,0) = 47;
    mat6(1,1) = 3;

    std::cout << mat6(0,1) << "\n";

    auto test2 = mat5*mat6;

    std::cout << test2(0,1) << "\n";
}
