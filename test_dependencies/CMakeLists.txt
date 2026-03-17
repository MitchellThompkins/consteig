if(CONSTEIG_COMPILE_ONLY)
    # Bare-metal cross-compilers can't build GoogleTest (needs POSIX).
    # Provide no-op stub headers so test TUs compile (exercising static_assert)
    # without requiring the hosted C++ standard library.
    set(_stub_inc ${CMAKE_CURRENT_SOURCE_DIR}/gtest_stubs)

    add_library(gtest INTERFACE)
    target_include_directories(gtest SYSTEM INTERFACE ${_stub_inc})

    add_library(gtest_main INTERFACE)
    target_include_directories(gtest_main SYSTEM INTERFACE ${_stub_inc})

    add_library(gmock INTERFACE)
    target_include_directories(gmock SYSTEM INTERFACE ${_stub_inc})

    add_library(gmock_main INTERFACE)
    target_include_directories(gmock_main SYSTEM INTERFACE ${_stub_inc})
else()
    add_subdirectory(googletest)
endif()

if(NOT CONSTEIG_COMPILE_ONLY)
    include(FetchContent)

    set(EIGEN_BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(EIGEN_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(EIGEN_BUILD_PKGCONFIG OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
      eigen
      GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
      GIT_TAG        3.4.0
    )
    FetchContent_MakeAvailable(eigen)

    set(BUILD_TESTING ON CACHE BOOL "" FORCE)
endif()
