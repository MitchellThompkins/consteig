add_subdirectory(googletest)

include(FetchContent)

# Comprehensively disable all Eigen tests and extra targets
set(EIGEN_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(EIGEN_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(EIGEN_BUILD_PKGCONFIG OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG        3.4.1
)
FetchContent_MakeAvailable(eigen)

# Re-enable testing for consteig
set(BUILD_TESTING ON CACHE BOOL "" FORCE)
