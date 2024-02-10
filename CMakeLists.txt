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
set(SRC_DIR src/)

set(INTERFACES spectral.c)
list(TRANSFORM INTERFACES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/)

set(INTERNALS d_array.c dimension.c renderer.c result.c shading.c windower.c)
list(TRANSFORM INTERNALS PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/internal/)

set(MODULES ${INTERNALS} ${INTERFACES})

# Dependencies
set(SCRIPT_DIR ${CMAKE_SOURCE_DIR}/scripts/cmake)
include(${SCRIPT_DIR}/setter.cmake)
include(${SCRIPT_DIR}/remotes.cmake)
include(${SCRIPT_DIR}/compile_tools.cmake)

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

# Set header locations
set(
    INTERNAL_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/internal/include
)
set(
    INTERFACE_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(${PROJECT_NAME} STATIC ${MODULES})
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${INTERNAL_INCLUDES}
    PUBLIC
    ${INTERFACE_INCLUDES}
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    ${DEPENDENCIES}
)

# Enable/disable and run tests
option(SPECTRAL_BUILD_TESTS "Include tests" OFF)
if(${SPECTRAL_BUILD_TESTS})
    message(STATUS "${PROJECT_NAME} tests are enabled")
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "${PROJECT_NAME} tests are disabled")
endif()

option(SPECTRAL_BUILD_EXAMPLES "Build example executables" ON)
if(${SPECTRAL_BUILD_EXAMPLES})
    message(STATUS "Example executables are enabled for ${PROJECT_NAME}")
    add_subdirectory(examples)
else()
    message(STATUS "Example executables are disabled for ${PROJECT_NAME}")
endif()

option(SPECTRAL_BUILD_APP "Build primary executable" ON)
if(${SPECTRAL_BUILD_APP})
    message(STATUS "Primary executable enabled for ${PROJECT_NAME}")
    add_subdirectory(app)
else()
    message(STATUS "Primary executable disabled for ${PROJECT_NAME}")
endif()
