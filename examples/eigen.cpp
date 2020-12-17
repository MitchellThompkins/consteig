#include "example_helper.hpp"
#include "test_tools.hpp"
#include "../consteig.hpp"

int main()
{
    static constexpr size_t s {4};

    static constexpr consteig::Matrix<double,s,s> mat
    {{{
        { -4.4529e-01,  4.9063e+00, -8.7871e-01,  6.3036e+00},
        { -6.3941e+00,  1.3354e+01,  1.6668e+00,  1.1945e+01},
        {  3.6842e+00, -6.6617e+00, -6.0021e-02, -7.0043e+00},
        {  3.1209e+00, -5.2052e+00, -1.4130e+00, -2.8484e+00}
    }}};

    //static constexpr auto eigenValueTest {eigvals(mat)};

    static constexpr consteig::Matrix<double,s,s> testEig {consteig::eig(mat)};
    static constexpr consteig::Matrix<double,s,1> testEigVals {consteig::eigvals(mat)};

    printMat("testEig", testEig);

    printMat("testEigVals", testEigVals);

    bool checkEigen = consteig::checkEigenValues<double,s,s>(mat, testEigVals, 0.00001);

    std::cout << checkEigen << "\n";
}
