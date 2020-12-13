#include "example_helper.hpp"

int main()
{
    static constexpr int x {3};
    static constexpr int s {4};

    // Create a 3x3 matrix
    static constexpr consteig::Matrix<int, x, x> mat1
    {{{{5, -4, 2}, {-1, 2 , 3}, {-2, 1, 0}}}};

    // Create a matrix which is the transpose
    static constexpr consteig::Matrix<int, x, x> mat2 {transpose(mat1)};

    // Multiply the two
    static constexpr consteig::Matrix<int, x, x> mat3 {mat1*mat2};

    // Multiply by a scalar
    static constexpr consteig::Matrix<int, x, x> mat4 {-3*mat2};
    //TODO(mthompkins): Investigate this
    //static constexpr consteig::Matrix<float, x, x> mat4 {3.4*static_cast<consteig::Matrix<float, x, x>>(mat2)};

    static constexpr consteig::Matrix<int, s, s> symmetricMat1
    {{{
        {-5, -4, 2, 1},
        {-4,  5, 7, 8},
        { 2,  7, 0, 3},
        { 1,  8, 3, 3}
    }}};

    static constexpr bool checkSymmetry {symmetricMat1.isSymmetric()};

    printMat("Mat1", mat1);
    printMat("Mat2", mat2);
    printMat("Mat3", mat3);
    printMat("Mat4", mat4);

    if(checkSymmetry)
        std::cout << "\nsymmetricMat1 is symmetric!\n";
    else
        std::cout << "\nsymmetricMat1 is not symmetric!\n";

}
