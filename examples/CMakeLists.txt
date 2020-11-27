################################################################################
# Executables
################################################################################
#add_library(matrix INTERFACE)

#TODO(mthompkins) Include in a better way
#include_directories(${PROJECT_SOURCE_DIR}/build/debug/include)

#target_include_directories(matrix INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(matrix.main ${CMAKE_CURRENT_SOURCE_DIR}/matrix.cpp)
target_link_libraries(matrix.main matrix array)
