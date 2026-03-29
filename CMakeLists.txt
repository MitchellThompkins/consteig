cmake_minimum_required(VERSION 3.13)

project(consteig)

################################################################################
# Public INTERFACE target (the only target external consumers need)
################################################################################
add_library(consteig INTERFACE)
target_include_directories(consteig INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_features(consteig INTERFACE cxx_std_17)
add_library(consteig::consteig ALIAS consteig)

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

    # For bare-metal cross-compilation there is no C runtime to link against,
    # so override add_executable to produce .o files only. Compilation still
    # exercises all static_assert checks. add_test is a no-op since there are
    # no executables for ctest to run.
    option(CONSTEIG_COMPILE_ONLY "Compile all targets without linking (for cross-compilation)" OFF)
    if(CONSTEIG_COMPILE_ONLY)
        macro(add_executable target)
            add_library(${target} OBJECT ${ARGN})
        endmacro()
        macro(add_test)
        endmacro()
    endif()


    option(CONSTEIG_BUILD_TESTS "Build tests and fetch test dependencies (Eigen, GoogleTest)" ON)

    # Test dependencies are fetched before consteig_warnings is defined so
    # third-party code is not compiled with strict warnings.
    if(CONSTEIG_BUILD_TESTS)
        add_subdirectory(test_dependencies)
        include_directories(SYSTEM ${CMAKE_BINARY_DIR}/_deps/eigen-src)
        enable_testing()
    endif()

    # Aggressive warnings and errors — linked explicitly to project targets so
    # flags do not propagate to external consumers via INTERFACE libraries.
    add_library(consteig_warnings INTERFACE)
    target_compile_options(consteig_warnings INTERFACE
        -Wall
        -Wextra
        -pedantic
        -Werror
        -Wmisleading-indentation
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Wundef
    )

    # GCC-specific warnings
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(consteig_warnings INTERFACE
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
        )
    endif()
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
    add_subdirectory(profiling EXCLUDE_FROM_ALL)

    if(CONSTEIG_BUILD_TESTS)
        add_subdirectory(array/tests)
        add_subdirectory(math/tests)
        add_subdirectory(matrix/tests)
        add_subdirectory(eigen/tests)
    endif()

endif()

