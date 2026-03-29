################################################################################
# Library
################################################################################
add_library(consteig_array INTERFACE)
target_include_directories(consteig_array INTERFACE
    ${CMAKE_SOURCE_DIR}/include/consteig/array
    ${CMAKE_SOURCE_DIR}/include
)
