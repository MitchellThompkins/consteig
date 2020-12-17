################################################################################
# Test
################################################################################
add_executable(matrix.test ${CMAKE_CURRENT_SOURCE_DIR}/matrix.test.cpp)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(matrix.test matrix array gtest_main)
create_test(matrix.test)

add_executable(operations.test ${CMAKE_CURRENT_SOURCE_DIR}/operations.test.cpp)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(operations.test matrix array gtest_main)
create_test(operations.test)

add_executable(householder.test ${CMAKE_CURRENT_SOURCE_DIR}/householder.test.cpp)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(householder.test matrix array gtest_main)
create_test(householder.test)

add_executable(hessenberg.test ${CMAKE_CURRENT_SOURCE_DIR}/hessenberg.test.cpp)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(hessenberg.test matrix array gtest_main)
create_test(hessenberg.test)

add_executable(qr.test ${CMAKE_CURRENT_SOURCE_DIR}/qr.test.cpp)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(qr.test matrix array gtest_main)
create_test(qr.test)
