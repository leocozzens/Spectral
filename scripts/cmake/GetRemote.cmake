function(GetRemote)
    cmake_parse_arguments(REMOTE "" "NAME;VERSION;URL;DESTINATION" "" ${ARGN})

    include(FetchContent)
    set(DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    FetchContent_Declare(
        ${REMOTE_NAME}
        URL                 ${REMOTE_URL}
        FIND_PACKAGE_ARGS   ${REMOTE_VERSION}
        SOURCE_DIR          ${REMOTE_DESTINATION}
    )
    FetchContent_MakeAvailable(${REMOTE_NAME})

    set("${REMOTE_NAME}_LOCATION" ${REMOTE_DESTINATION} PARENT_SCOPE)
endfunction()