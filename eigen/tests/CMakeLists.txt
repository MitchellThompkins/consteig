################################################################################
# Test
################################################################################
add_executable(consteig_eigen.test ${CMAKE_CURRENT_SOURCE_DIR}/eigen.test.cpp)
target_include_directories(consteig_eigen.test INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(consteig_eigen.test matrix array gtest_main Eigen3::Eigen consteig_eigen)
create_test(consteig_eigen.test)

add_executable(eigen_comparison.test ${CMAKE_CURRENT_SOURCE_DIR}/eigen_comparison.test.cpp)
target_include_directories(eigen_comparison.test INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(eigen_comparison.test matrix array gtest_main Eigen3::Eigen consteig_eigen)
create_test(eigen_comparison.test)
