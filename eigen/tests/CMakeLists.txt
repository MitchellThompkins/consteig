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

# Individual Non-Symmetric Tests
foreach(i RANGE 0 49)
    set(target_name "generated_nonsym_${i}")
    add_executable(${target_name}.test ${CMAKE_CURRENT_SOURCE_DIR}/${target_name}.test.cpp)
    target_compile_options(${target_name}.test PRIVATE -fconstexpr-ops-limit=1000000000)
    target_include_directories(${target_name}.test INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
    target_link_libraries(${target_name}.test matrix array gtest_main consteig_eigen)
    create_test(${target_name}.test)
endforeach()

# Individual Symmetric Tests
foreach(i RANGE 0 49)
    set(target_name "generated_sym_${i}")
    add_executable(${target_name}.test ${CMAKE_CURRENT_SOURCE_DIR}/${target_name}.test.cpp)
    target_compile_options(${target_name}.test PRIVATE -fconstexpr-ops-limit=1000000000)
    target_include_directories(${target_name}.test INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
    target_link_libraries(${target_name}.test matrix array gtest_main consteig_eigen)
    create_test(${target_name}.test)
endforeach()
