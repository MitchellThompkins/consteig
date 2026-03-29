################################################################################
# Test
################################################################################

add_executable(eigvecs_constexpr.test ${CMAKE_CURRENT_SOURCE_DIR}/eigvecs_constexpr.test.cpp)
target_include_directories(eigvecs_constexpr.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(eigvecs_constexpr.test consteig_matrix consteig_array gtest_main consteig_eigen consteig_warnings)
create_test(eigvecs_constexpr.test)

add_executable(eigvals_constexpr.test ${CMAKE_CURRENT_SOURCE_DIR}/eigvals_constexpr.test.cpp)
target_include_directories(eigvals_constexpr.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(eigvals_constexpr.test consteig_matrix consteig_array gtest_main consteig_eigen consteig_warnings)
create_test(eigvals_constexpr.test)

# These tests link against Eigen3 which is unavailable in cross-compilation
if(NOT CONSTEIG_COMPILE_ONLY)
    add_executable(comparison_eigen.test ${CMAKE_CURRENT_SOURCE_DIR}/comparison_eigen.test.cpp)
    target_include_directories(comparison_eigen.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
    target_link_libraries(comparison_eigen.test consteig_matrix consteig_array gtest_main Eigen3::Eigen consteig_eigen consteig_warnings)
    create_test(comparison_eigen.test)
endif()

# Helper function to add generated tests
function(add_gen_test target_name source_file)
    add_executable(${target_name}.test ${CMAKE_CURRENT_SOURCE_DIR}/${source_file})
    target_compile_definitions(${target_name}.test PRIVATE CONSTEIG_MAX_ITER=2000)
    target_include_directories(${target_name}.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
    target_link_libraries(${target_name}.test consteig_matrix consteig_array gtest_main consteig_eigen consteig_warnings)
    create_test(${target_name}.test)
endfunction()

# All Generated Tests
file(GLOB GENERATED_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/generated_*.test.cpp")
foreach(test_path ${GENERATED_TESTS})
    get_filename_component(test_file ${test_path} NAME)
    string(REPLACE ".test.cpp" "" target_name ${test_file})
    add_gen_test(${target_name} ${test_file})
endforeach()
