################################################################################
# Library
################################################################################
add_library(consteig_eigen INTERFACE)
target_include_directories(consteig_eigen INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

################################################################################
# Test
################################################################################
if(CONSTEIG_BUILD_TESTS)
    add_subdirectory(tests)
endif()
