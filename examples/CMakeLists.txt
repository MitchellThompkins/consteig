################################################################################
# Executables
################################################################################

add_executable(matrix.main ${CMAKE_CURRENT_SOURCE_DIR}/matrix.cpp)
target_link_libraries(matrix.main matrix array consteig_warnings)

add_executable(decomp.main ${CMAKE_CURRENT_SOURCE_DIR}/decomp.cpp)
target_link_libraries(decomp.main matrix array consteig_warnings)

add_executable(eigen.main ${CMAKE_CURRENT_SOURCE_DIR}/eigen.cpp)
target_link_libraries(eigen.main matrix array consteig_warnings)

add_executable(population.main ${CMAKE_CURRENT_SOURCE_DIR}/population.cpp)
target_link_libraries(population.main matrix array consteig_warnings)

add_executable(butterworth.main ${CMAKE_CURRENT_SOURCE_DIR}/butterworth.cpp ${CMAKE_CURRENT_SOURCE_DIR}/butterworth_values.cpp)
target_link_libraries(butterworth.main matrix array consteig_warnings)

# Excluded because building this SHOULD fail, but we don't want to fail the
# other tests
add_executable(dc_motor_control.main EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/dc_motor_control.cpp)
target_link_libraries(dc_motor_control.main matrix array consteig_warnings)

################################################################################
# Custom target to build only examples (without test binaries)
################################################################################
add_custom_target(examples DEPENDS matrix.main decomp.main eigen.main population.main butterworth.main)
