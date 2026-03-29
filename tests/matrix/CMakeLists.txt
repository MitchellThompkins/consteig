################################################################################
# Test
################################################################################
add_executable(matrix.test ${CMAKE_CURRENT_SOURCE_DIR}/matrix.test.cpp)
target_include_directories(matrix.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(matrix.test consteig_matrix gtest_main consteig_warnings)
create_test(matrix.test)

add_executable(complex_init.test ${CMAKE_CURRENT_SOURCE_DIR}/complex_init.test.cpp)
target_include_directories(complex_init.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(complex_init.test consteig_matrix gtest_main consteig_warnings)
create_test(complex_init.test)

add_executable(operations.test ${CMAKE_CURRENT_SOURCE_DIR}/operations.test.cpp)
target_include_directories(operations.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(operations.test consteig_matrix gtest_main consteig_warnings)
create_test(operations.test)

add_executable(householder.test ${CMAKE_CURRENT_SOURCE_DIR}/householder.test.cpp)
target_include_directories(householder.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(householder.test consteig_matrix gtest_main consteig_warnings)
create_test(householder.test)

add_executable(hessenberg.test ${CMAKE_CURRENT_SOURCE_DIR}/hessenberg.test.cpp)
target_include_directories(hessenberg.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(hessenberg.test consteig_matrix gtest_main consteig_warnings)
create_test(hessenberg.test)

add_executable(qr.test ${CMAKE_CURRENT_SOURCE_DIR}/qr.test.cpp)
target_include_directories(qr.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(qr.test consteig_matrix gtest_main consteig_warnings)
create_test(qr.test)

add_executable(lu.test ${CMAKE_CURRENT_SOURCE_DIR}/lu.test.cpp)
target_include_directories(lu.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(lu.test consteig_matrix gtest_main consteig_warnings)
create_test(lu.test)

if(NOT CONSTEIG_COMPILE_ONLY)
    add_executable(operations_eigen.test ${CMAKE_CURRENT_SOURCE_DIR}/operations_eigen.test.cpp)
    target_include_directories(operations_eigen.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
    target_link_libraries(operations_eigen.test consteig_matrix gtest_main Eigen3::Eigen consteig_warnings)
    create_test(operations_eigen.test)

    add_executable(householder_eigen.test ${CMAKE_CURRENT_SOURCE_DIR}/householder_eigen.test.cpp)
    target_include_directories(householder_eigen.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
    target_link_libraries(householder_eigen.test consteig_matrix gtest_main Eigen3::Eigen consteig_warnings)
    create_test(householder_eigen.test)

    add_executable(hessenberg_eigen.test ${CMAKE_CURRENT_SOURCE_DIR}/hessenberg_eigen.test.cpp)
    target_include_directories(hessenberg_eigen.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
    target_link_libraries(hessenberg_eigen.test consteig_matrix gtest_main Eigen3::Eigen consteig_warnings)
    create_test(hessenberg_eigen.test)

    add_executable(qr_eigen.test ${CMAKE_CURRENT_SOURCE_DIR}/qr_eigen.test.cpp)
    target_include_directories(qr_eigen.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
    target_link_libraries(qr_eigen.test consteig_matrix gtest_main Eigen3::Eigen consteig_warnings)
    create_test(qr_eigen.test)
endif()
