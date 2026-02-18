#ifndef EXAMPLER_HELPER_HPP
#define EXAMPLER_HELPER_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include "../consteig.hpp"

template<typename T, consteig::Size R, consteig::Size C>
void printMat( const std::string name, const consteig::Matrix<T,R,C> &mat)
{
    std::cout << "\n" << name << "\n";
    for(consteig::Size i {0}; i<R; i++)
    {
        for(consteig::Size j {0}; j<C; j++)
        {
            std::cout << std::setprecision(10) << mat(i,j) << " ";
        }
        std::cout << "\n";
    }
}

template<typename T, consteig::Size R, consteig::Size C>
void printMat( const std::string name, const consteig::Matrix<consteig::Complex<T>,R,C> &mat)
{
    std::cout << "\n" << name << "\n";
    for(consteig::Size i {0}; i<R; i++)
    {
        for(consteig::Size j {0}; j<C; j++)
        {
            std::cout << std::setprecision(10) << "(" << mat(i,j).real << "," << mat(i,j).imag << "i) ";
        }
        std::cout << "\n";
    }
}

#endif
