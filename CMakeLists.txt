cmake_minimum_required(VERSION 3.13)

project(consteig)

# Convenience function for raising constexpr limits on a specific target. Not
# used by default — the library's deflation criterion keeps iteration counts
# within default compiler limits. Users may call this on their own targets if
# needed for very large or pathological matrices.
function(consteig_raise_compiler_limits)
    foreach(target IN LISTS ARGN)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            target_compile_options(${target} PRIVATE -fconstexpr-ops-limit=1000000000)
        endif()
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_compile_options(${target} PRIVATE -fconstexpr-steps=1000000000)
        endif()
        target_compile_options(${target} PRIVATE -fconstexpr-depth=1024)
    endforeach()
endfunction()

macro(create_test target)
    add_test(NAME ${target} COMMAND ${target})
endmacro()

################################################################################
# Top-level-only settings (skipped when consumed via add_subdirectory/FetchContent)
################################################################################
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    option(CONSTEIG_COMPILE_ONLY "Compile all targets without linking (for cross-compilation)" OFF)
    if(CONSTEIG_COMPILE_ONLY)
        macro(add_executable target)
            add_library(${target} OBJECT ${ARGN})
        endmacro()
        macro(add_test)
        endmacro()
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

# Require C++17 minimum from consumers without overriding their standard
foreach(lib array math matrix consteig_eigen)
    target_compile_features(${lib} INTERFACE cxx_std_17)
endforeach()

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

    if(CONSTEIG_COMPILE_ONLY)
        add_executable(cross_compile_verify ${PROJECT_SOURCE_DIR}/cross_compile_verify.cpp)
        target_link_libraries(cross_compile_verify matrix array consteig_eigen)
    endif()
endif()
