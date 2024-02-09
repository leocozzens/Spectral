cmake_minimum_required(VERSION 3.27.9)

set(APP "${PROJECT_NAME}v${PROJECT_VERSION}")
set(APP_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(APP_SRCS main.c)
list(TRANSFORM APP_SRCS PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/)

add_executable(${APP} ${APP_SRCS})
target_link_libraries(
    ${APP} PRIVATE
    ${PROJECT_NAME}
)
target_include_directories(
    ${APP} PRIVATE
    ${APP_INCLUDES}
)