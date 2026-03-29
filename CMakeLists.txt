cmake_minimum_required(VERSION 3.13)

# Derive the project version from the nearest Git tag rather than hardcoding
# it here. This keeps GitHub release tags as the single source of truth:
# tagging a release (e.g. "1.2.3") is all that's needed to update the version
# exposed to find_package(consteig 1.2 CONFIG REQUIRED). Falls back to "0.0.0"
# in shallow clones or source tarballs where tags are unavailable.
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE _consteig_git_tag
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
        RESULT_VARIABLE _git_result
    )
    if(_git_result EQUAL 0)
        string(REGEX REPLACE "^v" "" _consteig_version "${_consteig_git_tag}")
    else()
        set(_consteig_version "0.0.0")
    endif()
else()
    set(_consteig_version "0.0.0")
endif()

project(consteig VERSION ${_consteig_version} LANGUAGES CXX)

include(GNUInstallDirs)

################################################################################
# Public INTERFACE target (the only target external consumers need)
################################################################################
add_library(consteig INTERFACE)
target_include_directories(consteig INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
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

################################################################################
# Library
################################################################################
add_subdirectory(array)
add_subdirectory(math)
add_subdirectory(matrix)
add_subdirectory(eigen)

################################################################################
# Development mode (skipped when consumed via add_subdirectory/FetchContent)
################################################################################
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(CONSTEIG_BUILD_TESTS "Build tests and fetch test dependencies (Eigen, GoogleTest)" OFF)
    option(CONSTEIG_BUILD_EXAMPLES "Build examples" OFF)
    option(CONSTEIG_BUILD_PROFILING "Build profiling targets" OFF)

    ################################
    # cmake options for dev
    ################################
    if(CONSTEIG_BUILD_TESTS OR CONSTEIG_BUILD_EXAMPLES OR CONSTEIG_BUILD_PROFILING)
        set(CMAKE_CXX_STANDARD 17)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    endif()


    ################################
    # Test configuration
    ################################
    if(CONSTEIG_BUILD_TESTS)
        macro(create_test target)
            add_test(NAME ${target} COMMAND ${target})
        endmacro()

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

        # Test dependencies are fetched before consteig_warnings is defined so
        # third-party code is not compiled with strict warnings.
        add_subdirectory(test_dependencies)
        include_directories(SYSTEM ${CMAKE_BINARY_DIR}/_deps/eigen-src)
        enable_testing()
    endif()

    ################################
    # Add warnings library for dev
    ################################
    if(CONSTEIG_BUILD_TESTS OR CONSTEIG_BUILD_EXAMPLES OR CONSTEIG_BUILD_PROFILING)
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
    endif()

    ################################
    # Examples, profiling, and tests
    ################################
    if(CONSTEIG_BUILD_EXAMPLES)
        add_subdirectory(examples EXCLUDE_FROM_ALL)
    endif()

    if(CONSTEIG_BUILD_PROFILING)
        add_subdirectory(profiling EXCLUDE_FROM_ALL)
    endif()

    if(CONSTEIG_BUILD_TESTS)
        add_subdirectory(array/tests)
        add_subdirectory(math/tests)
        add_subdirectory(matrix/tests)
        add_subdirectory(eigen/tests)
    endif()

endif()

################################################################################
# Install rules (top-level only)
################################################################################
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CMakePackageConfigHelpers)

    install(TARGETS consteig EXPORT consteigTargets)

    install(FILES
        consteig.hpp
        consteig_options.hpp
        consteig_types.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    install(DIRECTORY array/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/array FILES_MATCHING PATTERN "*.hpp")
    install(DIRECTORY math/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/math FILES_MATCHING PATTERN "*.hpp"
        PATTERN "tests" EXCLUDE)
    install(DIRECTORY matrix/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/matrix FILES_MATCHING PATTERN "*.hpp"
        PATTERN "tests" EXCLUDE)
    install(DIRECTORY eigen/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/eigen FILES_MATCHING PATTERN "*.hpp"
        PATTERN "tests" EXCLUDE)

    install(EXPORT consteigTargets
        FILE consteigTargets.cmake
        NAMESPACE consteig::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/consteig
    )

    configure_package_config_file(
        cmake/consteigConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/consteigConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/consteig
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/consteigConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/consteigConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/consteigConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/consteig
    )
endif()
