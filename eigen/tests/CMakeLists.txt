################################################################################
# Test
################################################################################
option(CONSTEIG_BUILD_SLOW_TESTS "Build slow (8x8) tests" OFF)
option(CONSTEIG_ENABLE_ROBUSTNESS "Enable intensive robustness tests" OFF)

if(CONSTEIG_ENABLE_ROBUSTNESS)
    add_compile_definitions(ENABLE_ROBUSTNESS)
endif()

add_executable(consteig_eigen.test ${CMAKE_CURRENT_SOURCE_DIR}/eigen.test.cpp)
target_include_directories(consteig_eigen.test INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_compile_options(consteig_eigen.test PRIVATE
    "$<$<CXX_COMPILER_ID:GNU>:-fconstexpr-ops-limit=1000000000>"
    "$<$<CXX_COMPILER_ID:Clang,AppleClang>:-fconstexpr-steps=1000000000>"
)
target_link_libraries(consteig_eigen.test matrix array gtest_main Eigen3::Eigen consteig_eigen)
create_test(consteig_eigen.test)

add_executable(eigen_comparison.test ${CMAKE_CURRENT_SOURCE_DIR}/eigen_comparison.test.cpp)
target_include_directories(eigen_comparison.test INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_compile_options(eigen_comparison.test PRIVATE
    "$<$<CXX_COMPILER_ID:GNU>:-fconstexpr-ops-limit=1000000000>"
    "$<$<CXX_COMPILER_ID:Clang,AppleClang>:-fconstexpr-steps=1000000000>"
)
target_link_libraries(eigen_comparison.test matrix array gtest_main Eigen3::Eigen consteig_eigen)
create_test(eigen_comparison.test)

# Helper function to add generated tests
function(add_gen_test target_name source_file)
    add_executable(${target_name}.test ${CMAKE_CURRENT_SOURCE_DIR}/${source_file})
    if(CONSTEIG_BUILD_SLOW_TESTS)
        target_compile_definitions(${target_name}.test PRIVATE CONSTEIG_SLOW_TESTS)
        target_compile_options(${target_name}.test PRIVATE
            "$<$<CXX_COMPILER_ID:GNU>:-fconstexpr-ops-limit=1000000000>"
            "$<$<CXX_COMPILER_ID:Clang,AppleClang>:-fconstexpr-steps=1000000000>"
        )
    else()
        target_compile_options(${target_name}.test PRIVATE
            "$<$<CXX_COMPILER_ID:GNU>:-fconstexpr-ops-limit=100000000>"
            "$<$<CXX_COMPILER_ID:Clang,AppleClang>:-fconstexpr-steps=100000000>"
        )
    endif()
    target_include_directories(${target_name}.test INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
    target_link_libraries(${target_name}.test matrix array gtest_main consteig_eigen)
    create_test(${target_name}.test)
endfunction()

# Individual Non-Symmetric Tests
foreach(i RANGE 0 99)
    set(target_name "generated_nonsym_${i}")
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${target_name}.test.cpp)
        add_gen_test(${target_name} ${target_name}.test.cpp)
    endif()
endforeach()

# Individual Symmetric Tests
foreach(i RANGE 0 99)
    set(target_name "generated_sym_${i}")
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${target_name}.test.cpp)
        add_gen_test(${target_name} ${target_name}.test.cpp)
    endif()
endforeach()

# Individual Robustness Tests
file(GLOB ROBUST_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/generated_robust_*.test.cpp")
foreach(test_path ${ROBUST_TESTS})
    get_filename_component(test_file ${test_path} NAME)
    string(REPLACE ".test.cpp" "" target_name ${test_file})
    add_gen_test(${target_name} ${test_file})
endforeach()
