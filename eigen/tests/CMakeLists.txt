################################################################################
# Test
################################################################################
consteig_raise_compiler_limits()

add_executable(consteig_eigen.test ${CMAKE_CURRENT_SOURCE_DIR}/eigen.test.cpp)
target_include_directories(consteig_eigen.test INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(consteig_eigen.test matrix array gtest_main Eigen3::Eigen consteig_eigen)
create_test(consteig_eigen.test)

add_executable(eigen_comparison.test ${CMAKE_CURRENT_SOURCE_DIR}/eigen_comparison.test.cpp)
target_include_directories(eigen_comparison.test INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(eigen_comparison.test matrix array gtest_main Eigen3::Eigen consteig_eigen)
create_test(eigen_comparison.test)

# Helper function to add generated tests
function(add_gen_test target_name source_file)
    add_executable(${target_name}.test ${CMAKE_CURRENT_SOURCE_DIR}/${source_file})
    target_compile_definitions(${target_name}.test PRIVATE CONSTEIG_MAX_ITER=2000)
    target_include_directories(${target_name}.test INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
    target_link_libraries(${target_name}.test matrix array gtest_main consteig_eigen)
    create_test(${target_name}.test)
endfunction()

# All Generated Tests
file(GLOB GENERATED_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/generated_*.test.cpp")
foreach(test_path ${GENERATED_TESTS})
    get_filename_component(test_file ${test_path} NAME)
    string(REPLACE ".test.cpp" "" target_name ${test_file})
    add_gen_test(${target_name} ${test_file})
endforeach()
