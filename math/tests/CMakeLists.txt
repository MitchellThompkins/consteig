################################################################################
# Test
################################################################################
add_executable(abs.test ${CMAKE_CURRENT_SOURCE_DIR}/abs.test.cpp)
target_include_directories(math INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
target_link_libraries(abs.test math gtest_main)
create_test(abs.test)

add_executable(pow.test ${CMAKE_CURRENT_SOURCE_DIR}/pow.test.cpp)
target_include_directories(math INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
target_link_libraries(pow.test math gtest_main)
create_test(pow.test)

add_executable(sgn.test ${CMAKE_CURRENT_SOURCE_DIR}/sgn.test.cpp)
target_include_directories(math INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
target_link_libraries(sgn.test math gtest_main)
create_test(sgn.test)

add_executable(sqrt.test ${CMAKE_CURRENT_SOURCE_DIR}/sqrt.test.cpp)
target_include_directories(math INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
target_link_libraries(sqrt.test math gtest_main)
create_test(sqrt.test)

add_executable(utilities.test ${CMAKE_CURRENT_SOURCE_DIR}/utilities.test.cpp)
target_include_directories(math INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
target_link_libraries(utilities.test math gtest_main)
create_test(utilities.test)
