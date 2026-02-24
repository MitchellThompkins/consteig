#include "../consteig.hpp"
#include "example_helper.hpp"

int main()
{
    // example from
    // https://medium.com/@andrew.chamberlain/using-eigenvectors-to-find-steady-state-population-flows-cd938f124764
    static constexpr consteig::Size pop_size{2};
    static constexpr consteig::Matrix<double, pop_size, pop_size> pop_mat{{{
        {0.95, 0.05},
        {0.20, 0.80},
    }}};

    static constexpr auto pop_eigs{consteig::eig(pop_mat)};
    static_assert(pop_eigs.sizeX() == pop_size,
                  "Eigen result should match input size");
    static constexpr auto pop_eig_vals{consteig::eigvals(pop_mat)};

    static constexpr auto pop_eig_vec {consteig::eigvecs(pop_mat, pop_eig_vals)};

    // For a Markov matrix describing population flow, the steady state is
    // represented by the eigenvector associated with the eigenvalue of 1.

    // First, we verify that one of the eigenvalues is 1.
    static_assert(
        consteig::abs(pop_eig_vals(0, 0).real - 1.0) < 1e-9 ||
            consteig::abs(pop_eig_vals(1, 0).real - 1.0) < 1e-9,
        "The matrix should have an eigenvalue of 1 to find a steady state.");

    printMat("Population Transition Matrix (A)", pop_mat);

    std::cout << "\nEigenvalues (lambda):" << std::endl;
    for (consteig::Size i = 0; i < pop_size; ++i)
    {
        std::cout << pop_eig_vals(i, 0).real << std::endl;
    }

    std::cout << "\nTo find the steady-state population distribution, we need "
                 "to find the eigenvector 'v'"
              << std::endl;
    std::cout << "that corresponds to the eigenvalue of 1 by solving (A - "
                 "lambda*I)v = 0."
              << std::endl;

    std::cout << "\nEigenvectors:" << std::endl;
    for (consteig::Size i = 0; i < pop_size; ++i)
    {
        std::cout << pop_eig_vec(i, 0).real << std::endl;
    }

    // TODO: The consteig library does not currently have a function to compute
    // eigenvectors from the Schur form (pop_eigs). The Schur form is an
    // intermediate step. A back-substitution algorithm would be needed to
    // extract the eigenvectors. The expected steady-state eigenvector for this
    // matrix is proportional to [0.8, 0.2].
}
