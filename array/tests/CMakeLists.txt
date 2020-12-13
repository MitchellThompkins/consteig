################################################################################
# Test
################################################################################
add_executable(array.test ${CMAKE_CURRENT_SOURCE_DIR}/array.test.cpp)
target_include_directories(array INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
target_link_libraries(array.test array gtest_main)
create_test(array.test)
