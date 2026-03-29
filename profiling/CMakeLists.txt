file(GLOB PROFILE_SOURCES "compile_time/profile_*.cpp")

foreach(src ${PROFILE_SOURCES})
    get_filename_component(target_name ${src} NAME_WE)
    add_library(${target_name} OBJECT ${src})
    target_include_directories(${target_name} PRIVATE "${CMAKE_SOURCE_DIR}/include")
    consteig_raise_compiler_limits(${target_name})
endforeach()
