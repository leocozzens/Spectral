cmake_minimum_required(VERSION 3.22.1)
project(Spectral VERSION 0.1 LANGUAGES C)

# General build settings
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -O0")
set(CMAKE_C_FLAGS_RELEASE "-O2")

# Build type detection
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Source files
set(SRCDIR src/)
set(MAIN_SRC main.c)
set(MODULES test.c)
list(TRANSFORM MAIN_SRC PREPEND ${SRCDIR})
list(TRANSFORM MODULES PREPEND ${SRCDIR})

# Dependencies
set(
    PRIMARY_INCLUDES 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set(DEPENDENCIES "")

# Enable/disable tests
option(INCLUDE_TESTS "Include tests" OFF)
message(STATUS "Tests are ${INCLUDE_TESTS}")

# Build primary executable and optionally execute tests
if(INCLUDE_TESTS)
    enable_testing()
    set(COMMON_LIB modules)
    add_library(${COMMON_LIB} STATIC ${MODULES})
    target_include_directories(
        ${COMMON_LIB} PUBLIC
        ${PRIMARY_INCLUDES}
    )
    add_subdirectory(tests)
    add_executable(${PROJECT_NAME} ${MAIN_SRC})
    target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        ${COMMON_LIB}
        ${DEPENDENCIES}
    )
else()
    set(SRCS ${MAIN_SRC} ${MODULES})
    add_executable(${PROJECT_NAME} ${SRCS})
    target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        ${DEPENDENCIES}
    )
    target_include_directories(
        ${PROJECT_NAME} PRIVATE
        ${PRIMARY_INCLUDES}
    )
endif()

# Set output directory primary executable
set(BINDIR ${CMAKE_BINARY_DIR}/bin)
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BINDIR}
)