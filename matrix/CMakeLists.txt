# Library
################################################################################
add_library(matrix INTERFACE)

#TODO(mthompkins) Include in a better way
#include_directories(${PROJECT_SOURCE_DIR}/build/debug/include)

target_include_directories(matrix INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#add_executable(matrix.main ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
#target_link_libraries(matrix.main matrix array)

# Test
################################################################################
#add_executable(matrix.test ${CMAKE_CURRENT_SOURCE_DIR}/matrix.test.cpp)
#target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
#target_link_libraries(matrix.test matrix array gtest_main)
#create_test(matrix.test)
