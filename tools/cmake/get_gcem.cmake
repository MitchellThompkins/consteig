function(get_gcem)
    set(options)
    set(oneValueArgs GIT_TAG INSTALL_DIR)
    set(multiValueArgs)
    cmake_parse_arguments(
        ARGS
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    # Set defaults for unprovided arguments
    if(NOT ARG_GIT_TAG)
        set(ARG_GIT_TAG "master")
    endif()

    if(NOT ARG_INSTALL_DIR)
        set(ARG_INSTALL_DIR ${CMAKE_BINARY_DIR})
    endif()

    set(PROJECT_NAME gcem)

    set(LIB_NAME_GCEM gcem)

    #set(LIB_LOCATION_GCEM
    #    ${ARG_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${LIB_NAME_GCEM}${CMAKE_STATIC_LIBRARY_SUFFIX}
    #)

    set(EXTERNAL_PROJ_NAME external_proj_${PROJECT_NAME})
    include(ExternalProject)

    externalproject_add(
        ${EXTERNAL_PROJ_NAME}
        GIT_REPOSITORY https://github.com/kthohr/gcem.git
        GIT_TAG ${ARG_GIT_TAG}
        INSTALL_DIR "${ARG_INSTALL_DIR}"
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        #BUILD_BYPRODUCTS
        #    ${LIB_LOCATION_GCEM}
    )

    find_package(Threads REQUIRED)

    set(LIB_INCLUDE_DIR ${ARG_INSTALL_DIR}/include)
    file(MAKE_DIRECTORY ${LIB_INCLUDE_DIR})

    #add_library(${LIB_NAME_GCEM} UNKNOWN IMPORTED)
    add_library(${LIB_NAME_GCEM} INTERFACE)
    target_include_directories(${LIB_NAME_GCEM} INTERFACE
        ${LIB_INCLUDE_DIR}
    )
    add_dependencies(${LIB_NAME_GCEM} ${EXTERNAL_PROJ_NAME})
    #set_target_properties(
    #    gcem
    #    PROPERTIES
    #        INTERFACE_INCLUDE_DIRECTORIES ${LIB_INCLUDE_DIR}
    #        IMPORTED_LINK_INTERFACE_LIBRARIES Threads::Threads
    #)

endfunction()
