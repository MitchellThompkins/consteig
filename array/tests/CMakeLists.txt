################################################################################
# Test
################################################################################
add_executable(array.test ${CMAKE_CURRENT_SOURCE_DIR}/array.test.cpp)
target_link_libraries(array.test consteig_array gtest_main consteig_warnings)
create_test(array.test)
