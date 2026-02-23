################################################################################
# Executables
################################################################################
consteig_raise_compiler_limits()

add_executable(matrix.main ${CMAKE_CURRENT_SOURCE_DIR}/matrix.cpp)
target_link_libraries(matrix.main matrix array)

add_executable(decomp.main ${CMAKE_CURRENT_SOURCE_DIR}/decomp.cpp)
target_link_libraries(decomp.main matrix array)

add_executable(eigen.main ${CMAKE_CURRENT_SOURCE_DIR}/eigen.cpp)
target_link_libraries(eigen.main matrix array)

add_executable(population.main ${CMAKE_CURRENT_SOURCE_DIR}/population.cpp)
target_link_libraries(population.main matrix array)

add_executable(butterworth.main ${CMAKE_CURRENT_SOURCE_DIR}/butterworth.cpp ${CMAKE_CURRENT_SOURCE_DIR}/butterworth_values.cpp)
target_link_libraries(butterworth.main matrix array)
