cmake_minimum_required(VERSION 3.27.9)

function(GetRemoteURL)
    cmake_parse_arguments(REMOTE "" "NAME;VERSION;URL;PARENT_DIR" "" ${ARGN})

    include(FetchContent)
    set(REMOTE_DESTINATION ${REMOTE_PARENT_DIR}/${REMOTE_NAME})
    FetchContent_Declare(
        ${REMOTE_NAME}
        URL                 ${REMOTE_URL}
        FIND_PACKAGE_ARGS   ${REMOTE_VERSION}
        SOURCE_DIR          ${REMOTE_DESTINATION}
    )
    FetchContent_MakeAvailable(${REMOTE_NAME})

    set("${REMOTE_NAME}_LOCATION" ${REMOTE_DESTINATION} PARENT_SCOPE)
endfunction()

function(GetRemoteTag)
    cmake_parse_arguments(REMOTE "" "NAME;PARENT_DIR;REPO;TAG" "" ${ARGN})

    include(FetchContent)
    set(REMOTE_DESTINATION ${REMOTE_PARENT_DIR}/${REMOTE_NAME})
    FetchContent_Declare(
        ${REMOTE_NAME}
        SOURCE_DIR          ${REMOTE_DESTINATION}
        GIT_REPOSITORY      ${REMOTE_REPO}
        GIT_TAG             ${REMOTE_TAG}
    )
    FetchContent_MakeAvailable(${REMOTE_NAME})

    set("${REMOTE_NAME}_LOCATION" ${REMOTE_DESTINATION} PARENT_SCOPE)
endfunction()

function(GetRepo)
    cmake_parse_arguments(REPO "" "NAME;VERSION;PATH;INCLUDES;SOURCES" "" ${ARGN})
    if(NOT REPO_PATH)
        set(REPO_PATH ${CMAKE_SOURCE_DIR}/external)
    endif()

    message(STATUS "REPO PATH: ${REPO_PATH}")
    set(REPO_DESTINATION ${REPO_PATH}/${REPO_NAME}/)
    list(TRANSFORM REPO_SOURCES PREPEND ${REPO_DESTINATION})

    add_library(
        ${REPO_NAME} STATIC
        ${REPO_SOURCES}
     )
    list(TRANSFORM REPO_INCLUDES PREPEND ${REPO_DESTINATION})
    target_include_directories(
        ${REPO_NAME} PUBLIC
        ${REPO_INCLUDES}
    )

    if(REPO_VERSION)
        set_target_properties(
            ${REPO_NAME} PROPERTIES
            VERSION ${REPO_VERSION}
        )
    endif()
endfunction()