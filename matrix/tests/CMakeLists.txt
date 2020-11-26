################################################################################
# Test
################################################################################
add_executable(matrix.test ${CMAKE_CURRENT_SOURCE_DIR}/matrix.test.cpp)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/dependencies/gcem/include)
target_link_libraries(matrix.test matrix array gtest_main)
create_test(matrix.test)
