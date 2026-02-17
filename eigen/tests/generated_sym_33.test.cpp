#include "generated_test_helpers.hpp"

TEST(generated_tests, sym_33)
{
    // static_assert(check_single_symmetric<33>(), "Sym 33 failed");
    if (!check_single_symmetric<33>()) {
        auto eigs = eigvals(mat_sym[33]);
        for (Size i=0; i<5; ++i) std::cout << "Comp 33: " << eigs(i,0).real << " + " << eigs(i,0).imag << "i" << std::endl;
        for (Size i=0; i<5; ++i) std::cout << "Exp 33: " << eigs_sym[33](i,0).real << " + " << eigs_sym[33](i,0).imag << "i" << std::endl;
    }
    EXPECT_TRUE(check_single_symmetric<33>());
}
