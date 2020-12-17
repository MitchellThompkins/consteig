################################################################################
# Test
################################################################################
add_executable(eigen.test ${CMAKE_CURRENT_SOURCE_DIR}/eigen.test.cpp)
target_include_directories(eigen INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(eigen.test matrix array gtest_main)
create_test(eigen.test)
