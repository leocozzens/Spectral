cmake_minimum_required(VERSION 3.27.9)
include(${CMAKE_SOURCE_DIR}/scripts/cmake/setter.cmake)

set(GLFW_VERSION 3.3.9)
set(DISABLES GLFW_BUILD_DOCS GLFW_BUILD_TESTS GLFW_BUILD_EXAMPLES)
DisableSettings("${DISABLES}")
GetRemoteURL(
    NAME        glfw
    VERSION     ${GLFW_VERSION}
    URL         https://github.com/glfw/glfw/releases/download/${GLFW_VERSION}/glfw-${GLFW_VERSION}.zip
    PARENT_DIR  ${CMAKE_CURRENT_BINARY_DIR}
)

GetRemoteTag(
    NAME        glad
    REPO        https://github.com/Dav1dde/glad.git
    TAG         master
    PARENT_DIR  ${CMAKE_CURRENT_BINARY_DIR}
)

set(DEPENDENCIES glfw glad PARENT_SCOPE)