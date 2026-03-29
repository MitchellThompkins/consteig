################################################################################
# Library
################################################################################
add_library(consteig_eigen INTERFACE)
target_include_directories(consteig_eigen INTERFACE
    ${CMAKE_SOURCE_DIR}/include/consteig/eigen
    ${CMAKE_SOURCE_DIR}/include
)
