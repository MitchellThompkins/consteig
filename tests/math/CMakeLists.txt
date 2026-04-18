################################################################################
# Test
################################################################################
add_executable(abs.test ${CMAKE_CURRENT_SOURCE_DIR}/abs.test.cpp)
target_include_directories(abs.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(abs.test consteig_math gtest_main consteig_warnings)
create_test(abs.test)

add_executable(pow.test ${CMAKE_CURRENT_SOURCE_DIR}/pow.test.cpp)
target_include_directories(pow.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(pow.test consteig_math gtest_main consteig_warnings)
create_test(pow.test)

add_executable(exp.test ${CMAKE_CURRENT_SOURCE_DIR}/exp.test.cpp)
target_include_directories(exp.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(exp.test consteig_math gtest_main consteig_warnings)
create_test(exp.test)

add_executable(sgn.test ${CMAKE_CURRENT_SOURCE_DIR}/sgn.test.cpp)
target_include_directories(sgn.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(sgn.test consteig_math gtest_main consteig_warnings)
create_test(sgn.test)

add_executable(sqrt.test ${CMAKE_CURRENT_SOURCE_DIR}/sqrt.test.cpp)
target_include_directories(sqrt.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(sqrt.test consteig_math gtest_main consteig_warnings)
create_test(sqrt.test)

add_executable(complex.test ${CMAKE_CURRENT_SOURCE_DIR}/complex.test.cpp)
target_include_directories(complex.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(complex.test consteig_math gtest_main consteig_warnings)
create_test(complex.test)

add_executable(trig.test ${CMAKE_CURRENT_SOURCE_DIR}/trig.test.cpp)
target_include_directories(trig.test PRIVATE ${PROJECT_SOURCE_DIR}/test_dependencies)
target_link_libraries(trig.test consteig_math gtest_main consteig_warnings)
create_test(trig.test)
