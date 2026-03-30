#include "example_helper.hpp"

int main()
{
    static constexpr int x{3};
    static constexpr int s{4};

    // Create a 3x3 matrix
    static constexpr consteig::Matrix<int, x, x> mat1{
        {{5, -4, 2}, {-1, 2, 3}, {-2, 1, 0}}};

    // Alternatively, use make_matrix with flat row-major arguments
    static constexpr consteig::Matrix<double, x, x> mat1Flat{
        consteig::make_matrix<double, x, x>(5.0, -4.0, 2.0, -1.0, 2.0, 3.0,
                                            -2.0, 1.0, 0.0)};

    // Create a matrix which is the transpose
    static constexpr consteig::Matrix mat2{transpose(mat1)};

    // Multiply the two
    static constexpr consteig::Matrix<int, x, x> mat3{mat1 * mat2};

    // Multiply by a scalar
    static constexpr consteig::Matrix<int, x, x> mat4{-3 * mat2};

    static constexpr consteig::Matrix<float, x, x> mat5{
        3.4f * consteig::matrix_cast<float>(mat2)};

    static constexpr consteig::Matrix<int, s, s> symmetricMat1{
        {{-5, -4, 2, 1}, {-4, 5, 7, 8}, {2, 7, 0, 3}, {1, 8, 3, 3}}};

    static constexpr consteig::Matrix<double, s, s> symmetricMat2{
        {{-5, -4, 2, 1}, {-4, 5, 7, 8}, {2, 7, 0, 3.32}, {1, 8, 3.32, 3.2}}};

    static constexpr bool checkSymmetryMat1{symmetricMat1.isSymmetric()};
    static constexpr bool checkSymmetryMat2{symmetricMat2.isSymmetric(0.1)};

    printMat("Mat1", mat1);
    printMat("Mat1 (via make_matrix)", mat1Flat);
    printMat("Mat2", mat2);
    printMat("Mat3", mat3);
    printMat("Mat4", mat4);
    printMat("Mat5", mat5);

    if (checkSymmetryMat1)
        std::cout << "\nsymmetricMat1 is symmetric!\n";
    else
        std::cout << "\nsymmetricMat1 is not symmetric!\n";

    if (checkSymmetryMat2)
        std::cout << "\nsymmetricMat2 is symmetric!\n";
    else
        std::cout << "\nsymmetricMat2 is not symmetric!\n";
}
