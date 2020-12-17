#ifndef EXAMPLER_HELPER_HPP
#define EXAMPLER_HELPER_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include "../consteig.hpp"

template<typename T, size_t R, size_t C>
void printMat( const std::string name, const consteig::Matrix<T,R,C> &mat)
{
    std::cout << "\n" << name << "\n";
    for(int i {0}; i<R; i++)
    {
        for(int j {0}; j<C; j++)
        {
            std::cout << std::setprecision(10) << mat(i,j) << " ";
        }
        std::cout << "\n";
    }
}

#endif
