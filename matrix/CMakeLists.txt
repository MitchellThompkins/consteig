################################################################################
# Library
################################################################################
add_library(consteig_matrix INTERFACE)
target_include_directories(consteig_matrix INTERFACE
    ${CMAKE_SOURCE_DIR}/include/consteig/matrix
    ${CMAKE_SOURCE_DIR}/include
)
