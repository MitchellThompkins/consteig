#include "../consteig.hpp"
#include "example_helper.hpp"

int main()
{
    // example from
    // https://medium.com/@andrew.chamberlain/using-eigenvectors-to-find-steady-state-population-flows-cd938f124764
    static constexpr consteig::Size pop_size{2};
    // The matrix must be left-stochastic (columns sum to 1) for
    // right-multiplication (A*v).
    static constexpr consteig::Matrix<double, pop_size, pop_size> pop_mat{{{
        {0.95, 0.20},
        {0.05, 0.80},
    }}};

    static constexpr consteig::Matrix<double, pop_size, 1> u0{{{
        {500000.0},
        {500000.0},
    }}};

    static constexpr auto pop_eigs{consteig::eig(pop_mat)};
    static_assert(pop_eigs.rows() == pop_size,
                  "Eigen result should match input size");
    static constexpr auto pop_eig_vals{consteig::eigvals(pop_mat)};

    static constexpr auto pop_eig_vec{consteig::eigvecs(pop_mat, pop_eig_vals)};

    // For a Markov matrix describing population flow, the steady state is
    // represented by the eigenvector associated with the eigenvalue of 1.

    // First, we verify that one of the eigenvalues is 1.
    consteig::Size steady_state_idx = 0;
    bool found_steady_state = false;
    for (consteig::Size i = 0; i < pop_size; ++i)
    {
        if (consteig::abs(pop_eig_vals(i, 0).real - 1.0) < 1e-6)
        {
            steady_state_idx = i;
            found_steady_state = true;
            break;
        }
    }

    if (!found_steady_state)
    {
        std::cerr << "Error: No steady state (eigenvalue = 1) found."
                  << std::endl;
        return 1;
    }

    printMat("Population Transition Matrix (A)", pop_mat);
    printMat("Initial Population Vector (u0)", u0);

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

    double total_population = 0;
    for (consteig::Size i = 0; i < pop_size; ++i)
    {
        total_population += u0(i, 0);
    }

    // Calculate sum of the steady-state eigenvector components to normalize to
    // 100%
    double sum = 0;
    for (consteig::Size i = 0; i < pop_size; ++i)
    {
        sum += consteig::abs(pop_eig_vec(i, steady_state_idx).real);
    }

    std::cout << "\nSteady-State Population Flow (Total: " << total_population
              << "):" << std::endl;
    for (consteig::Size i = 0; i < pop_size; ++i)
    {
        // Normalize the Euclidean eigenvector into a population distribution
        // (sum = 1)
        double proportion =
            consteig::abs(pop_eig_vec(i, steady_state_idx).real) / sum;
        double absolute = proportion * total_population;

        std::cout << (i == 0 ? "Seattle:  " : "Portland: ") << std::fixed
                  << std::setprecision(0) << absolute << " ("
                  << std::setprecision(2) << (proportion * 100.0) << "%)"
                  << std::endl;
    }

    return 0;
}
