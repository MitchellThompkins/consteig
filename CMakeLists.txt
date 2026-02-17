cmake_minimum_required(VERSION 3.11)

project(eigenvalue_solver)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF)

#add_compile_options(-Wall -Wextra -pedantic)# -Werror)

#TODO(mthompkins): Investigate if this is doing what I think it is
add_compile_options(-nostdlib -nodefaultlibs -lgcc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/bin)

macro(create_test target)
    add_test(NAME ${target} COMMAND ${target})
endmacro()

add_subdirectory(test_dependencies)

# Enable testing AFTER test_dependencies so Eigen doesn't see it enabled
enable_testing()
add_subdirectory(array)
add_subdirectory(math)
add_subdirectory(matrix)
add_subdirectory(eigen)
add_subdirectory(examples)
