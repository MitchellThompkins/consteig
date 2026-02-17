#include "example_helper.hpp"
#include "../consteig.hpp"

int main()
{
    // example from
    // https://medium.com/@andrew.chamberlain/using-eigenvectors-to-find-steady-state-population-flows-cd938f124764
    static constexpr consteig::Size pop_size {2};
    static constexpr consteig::Matrix<double,pop_size,pop_size> pop_mat
    {{{
        { 0.95, 0.05},
        { 0.20, 0.80},
    }}};

    static constexpr auto pop_eig_vals {consteig::eigvals(pop_mat)};

    printMat("pop_mat", pop_mat);
    
    std::cout << "pop_eig_vals:" << std::endl;
    for(consteig::Size i=0; i<pop_size; ++i) {
        std::cout << "(" << pop_eig_vals(i,0).real << ", " << pop_eig_vals(i,0).imag << "i)" << std::endl;
    }
}
