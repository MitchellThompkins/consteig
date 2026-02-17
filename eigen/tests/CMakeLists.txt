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

add_executable(generated_data.test ${CMAKE_CURRENT_SOURCE_DIR}/generated.test.cpp)
target_include_directories(generated_data.test INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(generated_data.test matrix array gtest_main consteig_eigen)
create_test(generated_data.test)
