#include <consteig/consteig.hpp>
#include "example_helper.hpp"

int main()
{
    static constexpr consteig::Size s{4};

    static constexpr consteig::Matrix<double, s, s> mat{
        {{-4.4529e-01, 4.9063e+00, -8.7871e-01, 6.3036e+00},
         {-6.3941e+00, 1.3354e+01, 1.6668e+00, 1.1945e+01},
         {3.6842e+00, -66617e+00, -6.0021e-02, -7.0043e+00},
         {3.1209e+00, -5.2052e+00, -1.4130e+00, -2.8484e+00}}};

    static constexpr consteig::Matrix<double, s, s> testEig{consteig::eig(mat)};
    static constexpr consteig::Matrix<consteig::Complex<double>, s, 1>
        testEigVals{consteig::eigenvalues(mat)};

    printMat("testEig (Schur Form)", testEig);

    std::cout << "testEigVals:\n";
    for (consteig::Size i = 0; i < s; ++i)
    {
        std::cout << "(" << testEigVals(i, 0).real << ", "
                  << testEigVals(i, 0).imag << "i)\n";
    }

    bool checkEigen =
        consteig::checkEigenValues<double, s, s>(mat, testEigVals, 0.00001);

    std::cout << "Valid eigenvalues: " << std::boolalpha << checkEigen << "\n";
}
