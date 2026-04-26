cmake_minimum_required(VERSION 3.13)

# Derive the project version from the nearest Git tag rather than hardcoding
# it here. This keeps GitHub release tags as the single source of truth:
# tagging a release (e.g. "1.2.3") is all that's needed to update the version
# exposed to find_package(consteig 1.2 CONFIG REQUIRED).
#
# vcpkg port maintainer note: vcpkg builds from a tarball or shallow clone
# where git tags are unavailable, so the git detection below will fall back to
# "0.0.0". Pass -DCONSTEIG_VERSION=<version> in the portfile's
# vcpkg_cmake_configure call to inject the correct version. End users set
# nothing — this is handled entirely in the portfile.
#
# Falls back to "0.0.0" when neither CONSTEIG_VERSION nor a git tag is
# available.
if(DEFINED CONSTEIG_VERSION)
    set(_consteig_version "${CONSTEIG_VERSION}")
else()
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
endif()

project(consteig VERSION ${_consteig_version} LANGUAGES CXX)

include(GNUInstallDirs)

################################################################################
# Public INTERFACE target (the only target external consumers need)
################################################################################
add_library(consteig INTERFACE)
target_include_directories(consteig INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(consteig INTERFACE cxx_std_17)
add_library(consteig::consteig ALIAS consteig)

################################################################################
# Optional gcem math backend
################################################################################
option(CONSTEIG_USE_GCEM "Use gcem math instead of built-in constexpr math" OFF)
option(CONSTEIG_GCEM_USE_STDLIB "Allow gcem to use stdlib headers (only with CONSTEIG_USE_GCEM)" OFF)
option(CONSTEIG_GCEM_USE_CUSTOM_TRAITS "Let user supply gcem trait definitions in namespace gcem (only with CONSTEIG_USE_GCEM)" OFF)

if(CONSTEIG_GCEM_USE_STDLIB AND NOT CONSTEIG_USE_GCEM)
    message(WARNING "CONSTEIG_GCEM_USE_STDLIB has no effect without CONSTEIG_USE_GCEM")
endif()

if(CONSTEIG_GCEM_USE_CUSTOM_TRAITS AND NOT CONSTEIG_USE_GCEM)
    message(WARNING "CONSTEIG_GCEM_USE_CUSTOM_TRAITS has no effect without CONSTEIG_USE_GCEM")
endif()

if(CONSTEIG_GCEM_USE_STDLIB AND CONSTEIG_GCEM_USE_CUSTOM_TRAITS)
    message(FATAL_ERROR "CONSTEIG_GCEM_USE_STDLIB and CONSTEIG_GCEM_USE_CUSTOM_TRAITS are mutually exclusive")
endif()

if(CONSTEIG_USE_GCEM)
    target_compile_definitions(consteig INTERFACE CONSTEIG_USE_GCEM)
    if(CONSTEIG_GCEM_USE_STDLIB)
        target_compile_definitions(consteig INTERFACE CONSTEIG_GCEM_USE_STDLIB)
    elseif(CONSTEIG_GCEM_USE_CUSTOM_TRAITS)
        target_compile_definitions(consteig INTERFACE CONSTEIG_GCEM_USE_CUSTOM_TRAITS)
    endif()
    target_include_directories(consteig SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/consteig/optional_dependencies>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/consteig/optional_dependencies>
    )
endif()

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
add_library(consteig_math INTERFACE)
target_include_directories(consteig_math INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/consteig/math
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(consteig_matrix INTERFACE)
target_include_directories(consteig_matrix INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/consteig/matrix
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(consteig_eigen INTERFACE)
target_include_directories(consteig_eigen INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/consteig/eigen
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

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
            if(CONSTEIG_COVERAGE)
                target_link_libraries(${target} consteig_coverage)
            endif()
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

        option(CONSTEIG_COVERAGE "Enable gcov/lcov code coverage instrumentation" OFF)

        if(CONSTEIG_COVERAGE)
            if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
                message(FATAL_ERROR "CONSTEIG_COVERAGE requires GCC or Clang")
            endif()

            add_library(consteig_coverage INTERFACE)

            if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
                target_compile_options(consteig_coverage INTERFACE
                    --coverage -O0 -fno-inline -fno-inline-small-functions -fno-default-inline)
            else()
                target_compile_options(consteig_coverage INTERFACE
                    --coverage -O0 -fno-inline)
            endif()
            target_link_options(consteig_coverage INTERFACE --coverage)

            find_program(LCOV_EXECUTABLE lcov REQUIRED)
            find_program(GENHTML_EXECUTABLE genhtml REQUIRED)

            add_custom_target(coverage
                COMMAND ${LCOV_EXECUTABLE}
                    --capture
                    --ignore-errors inconsistent
                    --directory ${CMAKE_BINARY_DIR}
                    --output-file ${CMAKE_BINARY_DIR}/coverage.info
                COMMAND ${LCOV_EXECUTABLE}
                    --remove ${CMAKE_BINARY_DIR}/coverage.info
                    "${CMAKE_BINARY_DIR}/_deps/*"
                    "/usr/*"
                    "${PROJECT_SOURCE_DIR}/test_dependencies/googletest/*"
                    --output-file ${CMAKE_BINARY_DIR}/coverage_filtered.info
                COMMAND ${GENHTML_EXECUTABLE}
                    ${CMAKE_BINARY_DIR}/coverage_filtered.info
                    --output-directory ${CMAKE_BINARY_DIR}/coverage_html
                COMMAND ${CMAKE_COMMAND} -E echo
                    "Coverage report: ${CMAKE_BINARY_DIR}/coverage_html/index.html"
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating HTML coverage report"
            )
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
        add_subdirectory(tests)
    endif()

endif()

################################################################################
# Install rules (top-level only)
################################################################################
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CMakePackageConfigHelpers)

    install(TARGETS consteig EXPORT consteigTargets)

    install(DIRECTORY include/consteig DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

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
