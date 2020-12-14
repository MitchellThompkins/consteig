################################################################################
# Test
################################################################################
add_executable(abs.test ${CMAKE_CURRENT_SOURCE_DIR}/abs.test.cpp)
target_include_directories(math INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
target_link_libraries(abs.test math gtest_main)
create_test(abs.test)
