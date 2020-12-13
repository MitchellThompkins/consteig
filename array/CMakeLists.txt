# Library
################################################################################
add_library(array INTERFACE)
target_include_directories(array INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

################################################################################
# Test
################################################################################
add_subdirectory(tests)
