function(get_google_test)
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

    set(PROJECT_NAME googletest)

    set(LIB_NAME_GTEST gtest)
    set(LIB_NAME_GTEST_MAIN gtest_main)
    set(LIB_NAME_GMOCK gmock)
    set(LIB_NAME_GMOCK_MAIN gmock_main)

    set(LIB_LOCATION_GTEST
        ${ARG_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${LIB_NAME_GTEST}${CMAKE_STATIC_LIBRARY_SUFFIX}
    )
    set(LIB_LOCATION_GTEST_MAIN
        ${ARG_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${LIB_NAME_GTEST_MAIN}${CMAKE_STATIC_LIBRARY_SUFFIX}
    )
    set(LIB_LOCATION_GMOCK
        ${ARG_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${LIB_NAME_GMOCK}${CMAKE_STATIC_LIBRARY_SUFFIX}
    )
    set(LIB_LOCATION_GMOCK_MAIN
        ${ARG_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${LIB_NAME_GMOCK_MAIN}${CMAKE_STATIC_LIBRARY_SUFFIX}
    )

    set(EXTERNAL_PROJ_NAME external_proj_${PROJECT_NAME})
    include(ExternalProject)

    externalproject_add(
        ${EXTERNAL_PROJ_NAME}
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG ${ARG_GIT_TAG}
        INSTALL_DIR "${ARG_INSTALL_DIR}"
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        BUILD_BYPRODUCTS
            ${LIB_LOCATION_GTEST}
            ${LIB_LOCATION_GTEST_MAIN}
            ${LIB_LOCATION_GMOCK}
            ${LIB_LOCATION_GMOCK_MAIN}
    )

    find_package(Threads REQUIRED)

    set(LIB_INCLUDE_DIR ${ARG_INSTALL_DIR}/include)
    file(MAKE_DIRECTORY ${LIB_INCLUDE_DIR})

    add_library(${LIB_NAME_GTEST} UNKNOWN IMPORTED)
    add_dependencies(${LIB_NAME_GTEST} ${EXTERNAL_PROJ_NAME})
    set_target_properties(
        gtest
        PROPERTIES
            IMPORTED_LOCATION ${LIB_LOCATION_GTEST}
            INTERFACE_INCLUDE_DIRECTORIES ${LIB_INCLUDE_DIR}
            IMPORTED_LINK_INTERFACE_LIBRARIES Threads::Threads
    )

    add_library(${LIB_NAME_GTEST_MAIN} UNKNOWN IMPORTED)
    add_dependencies(${LIB_NAME_GTEST_MAIN} ${EXTERNAL_PROJ_NAME})
    set_target_properties(
        ${LIB_NAME_GTEST_MAIN}
        PROPERTIES
            IMPORTED_LOCATION ${LIB_LOCATION_GTEST_MAIN}
            INTERFACE_INCLUDE_DIRECTORIES ${LIB_INCLUDE_DIR}
            IMPORTED_LINK_INTERFACE_LIBRARIES "Threads::Threads;gtest"
    )

    add_library(${LIB_NAME_GMOCK} UNKNOWN IMPORTED)
    add_dependencies(${LIB_NAME_GMOCK} ${EXTERNAL_PROJ_NAME})
    set_target_properties(
        ${LIB_NAME_GMOCK}
        PROPERTIES
            IMPORTED_LOCATION ${LIB_LOCATION_GMOCK}
            INTERFACE_INCLUDE_DIRECTORIES ${LIB_INCLUDE_DIR}
            IMPORTED_LINK_INTERFACE_LIBRARIES Threads::Threads
    )

    add_library(${LIB_NAME_GMOCK_MAIN} UNKNOWN IMPORTED)
    add_dependencies(${LIB_NAME_GMOCK_MAIN} ${EXTERNAL_PROJ_NAME})
    set_target_properties(
        ${LIB_NAME_GMOCK_MAIN}
        PROPERTIES
            IMPORTED_LOCATION ${LIB_LOCATION_GMOCK_MAIN}
            INTERFACE_INCLUDE_DIRECTORIES ${LIB_INCLUDE_DIR}
            IMPORTED_LINK_INTERFACE_LIBRARIES "Threads::Threads;gmock"
    )
endfunction()
