################################################################################
# Test
################################################################################
add_executable(eigen.test ${CMAKE_CURRENT_SOURCE_DIR}/eigen.test.cpp)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/dependencies/gcem/include)
target_link_libraries(eigen.test matrix array gtest_main)
create_test(eigen.test)
