################################################################################
# Test
################################################################################
add_executable(matrix.test ${CMAKE_CURRENT_SOURCE_DIR}/matrix.test.cpp)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/dependencies/gcem/include)
target_link_libraries(matrix.test matrix array gtest_main)
create_test(matrix.test)

add_executable(operations.test ${CMAKE_CURRENT_SOURCE_DIR}/operations.test.cpp)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/dependencies/gcem/include)
target_link_libraries(operations.test matrix array gtest_main)
create_test(operations.test)

#add_executable(decompositions.test ${CMAKE_CURRENT_SOURCE_DIR}/decompositions.test.cpp)
#target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/tools/gtest)
#target_include_directories(matrix INTERFACE ${PROJECT_SOURCE_DIR}/dependencies/gcem/include)
#target_link_libraries(decompositions.test matrix array gtest_main)
#create_test(decompositions.test)
