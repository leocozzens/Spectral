function(SetCompileDir PACKAGE_NAME COMPILE_DIR)
    set_target_properties(
        ${PACKAGE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${COMPILE_DIR}
    )
endfunction()

function(CompileExample)
    cmake_parse_arguments(EXAMPLE "" "NAME;INCLUDES;SRCS" "" ${ARGN})

    list(TRANSFORM EXAMPLE_SRCS PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_NAME}/)
    set(EXAMPLE_NAME "${PROJECT_NAME}_${EXAMPLE_NAME}")
    
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SRCS})
    target_link_libraries(
        ${EXAMPLE_NAME} PRIVATE
        ${PROJECT_NAME}
    )
    target_include_directories(
        ${EXAMPLE_NAME} PRIVATE
        ${EXAMPLE_INCLUDES}
    )

    SetCompileDir(${EXAMPLE_NAME} bin)
endfunction()

function(CompileApp)
    cmake_parse_arguments(APP "" "NAME;INCLUDES;SRC_DIR;SRCS" "" ${ARGN})

    list(TRANSFORM APP_SRCS PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/${APP_SRC_DIR}/)
    add_executable(${APP_NAME} ${APP_SRCS})
    target_link_libraries(
        ${APP_NAME} PRIVATE
        ${PROJECT_NAME}
    )
    target_include_directories(
        ${APP_NAME} PRIVATE
        ${APP_INCLUDES}
    )
    SetCompileDir(${APP_NAME} ${APP_SRC_DIR})
endfunction()