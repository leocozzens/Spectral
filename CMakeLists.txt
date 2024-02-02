# CMake version settings
cmake_minimum_required(VERSION 3.27.9)
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.18)
    cmake_policy(SET CMP0135 NEW)
endif()
# Project data
project(Spectral VERSION 0.1 LANGUAGES C)

# General build settings
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -O0")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

# Build type detection
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Source files
set(SRCDIR src/)
set(MAIN_SRC main.c)
set(MODULES dimension.c renderer.c result.c shader_tools.c windower.c)
list(TRANSFORM MAIN_SRC PREPEND ${SRCDIR})
list(TRANSFORM MODULES PREPEND ${SRCDIR})

# Dependencies
include(scripts/cmake/setter.cmake)
include(scripts/cmake/remotes.cmake)

set(LIB_DIR ${CMAKE_BINARY_DIR}/lib)
set(DISABLES GLFW_BUILD_DOCS GLFW_BUILD_TESTS GLFW_BUILD_EXAMPLES)
DisableSettings("${DISABLES}")

set(GLFW_VERSION 3.3.9)
GetRemoteURL(
    NAME        glfw
    VERSION     ${GLFW_VERSION}
    URL         https://github.com/glfw/glfw/releases/download/${GLFW_VERSION}/glfw-${GLFW_VERSION}.zip
    PARENT_DIR  ${LIB_DIR}
)
# Add GL versioning
GetRemoteTag(
    NAME        glad
    REPO        https://github.com/Dav1dde/glad.git
    TAG         master
    PARENT_DIR  ${LIB_DIR}
)

set(DEPENDENCIES glfw glad)
# Set primary includes directory
set(
    PRIMARY_INCLUDES 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Enable/disable tests
option(INCLUDE_TESTS "Include tests" OFF)
message(STATUS "Project tests are ${INCLUDE_TESTS}")

# Build primary executable and optionally execute tests
if(INCLUDE_TESTS)
    enable_testing()
    set(COMMON_LIB modules)
    add_library(${COMMON_LIB} STATIC ${MODULES})
    target_include_directories(
        ${COMMON_LIB} PUBLIC
        ${PRIMARY_INCLUDES}
    )
    target_link_libraries(
        ${COMMON_LIB} PUBLIC
        ${DEPENDENCIES}
    )
    add_subdirectory(tests)
    add_executable(${PROJECT_NAME} ${MAIN_SRC})
    target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        ${COMMON_LIB}
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