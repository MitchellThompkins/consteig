cmake_minimum_required(VERSION 3.13)

project(consteig)

# Note: The following limits are applied to allow for deeper constexpr functionality,
# such as complex eigenvalue calculations which exceed default compiler limits.
# For GCC, -fconstexpr-ops-limit is increased from the default 33M to 1B.
# For Clang, -fconstexpr-steps is used for the same purpose.
macro(consteig_raise_compiler_limits)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compile_options(-fconstexpr-ops-limit=1000000000)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-fconstexpr-steps=1000000000)
    endif()
    add_compile_options(-fconstexpr-depth=1024)
endmacro()

macro(create_test target)
    add_test(NAME ${target} COMMAND ${target})
endmacro()

################################################################################
# Top-level-only settings (skipped when consumed via add_subdirectory/FetchContent)
################################################################################
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 17)
    set(CXX_STANDARD_REQUIRED)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    option(CONSTEIG_RAISE_COMPILER_LIMITS "Raise constexpr limits for all targets" OFF)
    if(CONSTEIG_RAISE_COMPILER_LIMITS)
        consteig_raise_compiler_limits()
    endif()

    option(CONSTEIG_BUILD_TESTS "Build tests and fetch test dependencies (Eigen, GoogleTest)" ON)

    # Test dependencies are fetched before -Werror so third-party code is not
    # compiled with strict warnings.
    if(CONSTEIG_BUILD_TESTS)
        add_subdirectory(test_dependencies)
        enable_testing()
    endif()

    add_compile_options(-Wall -Wextra -pedantic -Werror)
else()
    option(CONSTEIG_BUILD_TESTS "Build tests and fetch test dependencies (Eigen, GoogleTest)" OFF)
endif()

################################################################################
# Library
################################################################################
add_subdirectory(array)
add_subdirectory(math)
add_subdirectory(matrix)
add_subdirectory(eigen)

################################################################################
# Examples and tests (top-level only)
################################################################################
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(examples EXCLUDE_FROM_ALL)

    if(CONSTEIG_BUILD_TESTS)
        add_subdirectory(array/tests)
        add_subdirectory(math/tests)
        add_subdirectory(matrix/tests)
        add_subdirectory(eigen/tests)
    endif()
endif()
