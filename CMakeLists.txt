cmake_minimum_required(VERSION 3.13)

project(eigenvalue_solver)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic -Werror)

# Note: The following limits are applied to allow for deeper constexpr functionality, 
# such as complex eigenvalue calculations which exceed default compiler limits.
# For GCC, -fconstexpr-ops-limit is increased from the default 33M to 1B.
# For Clang, -fconstexpr-steps is used for the same purpose.
option(CONSTEIG_RAISE_COMPILER_LIMITS "Raise constexpr limits for all targets" OFF)
if(CONSTEIG_RAISE_COMPILER_LIMITS)
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compile_options(-fconstexpr-ops-limit=1000000000)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-fconstexpr-steps=1000000000)
    endif()
    add_compile_options(-fconstexpr-depth=1024)
endif()

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
