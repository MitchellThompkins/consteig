# Library
################################################################################
add_library(qr_decomp INTERFACE)

#TODO(mthompkins) Include in a better way
include_directories(${PROJECT_SOURCE_DIR}/build/debug/include)

target_include_directories(qr_decomp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(qrDecomp.main ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(qrDecomp.main qr_decomp matrix array)

# Test
################################################################################
add_executable(qr_decomp.test ${CMAKE_CURRENT_SOURCE_DIR}/qr_decomp.test.cpp)
target_link_libraries(qr_decomp.test qr_decomp matrix array gtest_main)
create_test(qr_decomp.test)
