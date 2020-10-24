# Library
################################################################################
add_library(qr_decomp INTERFACE)
target_include_directories(qr_decomp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#add_executable(solver.main ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
#target_link_libraries(solver.main matrix array)

# Test
################################################################################
add_executable(qr_decomp.test ${CMAKE_CURRENT_SOURCE_DIR}/qr_decomp.test.cpp)
target_link_libraries(qr_decomp.test matrix array gtest_main)
create_test(qr_decomp.test)
