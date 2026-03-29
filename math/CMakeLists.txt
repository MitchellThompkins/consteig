################################################################################
# Library
################################################################################
add_library(consteig_math INTERFACE)
target_include_directories(consteig_math INTERFACE
    ${CMAKE_SOURCE_DIR}/include/consteig/math
    ${CMAKE_SOURCE_DIR}/include
)
