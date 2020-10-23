#include "matrix.hpp"
#include <iostream>

int main()
{
    static constexpr int r {3};
    static constexpr int c {2};

    static constexpr int scalar {2};
    /*static constexpr*/ Matrix<int, r, c> mat {{{{3, 4}, {5, 6}, {3, 9}}}};

    static constexpr Matrix<int, r, c> answer
    {{{{6, 8}, {10, 12}, {6, 18}}}};

    /*static constexpr*/ Matrix<int, r, c> mult { scalar*mat };

    for(size_t i {0}; i<r; i++)
    {
        for(size_t j {0}; j<c; j++)
        {
            std::cout << mult(i,j) << " ";
        }
            std::cout << "\n";
    }
}
