cmake_minimum_required(VERSION 3.22.1)

# Sources and test naming
set(NAME "run_tests")
set(TEST_SRCS demo_tests.c)
set(FRAMEWORK Unity)

# Assemble the testing framework
add_library(
    ${FRAMEWORK} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendors/Unity/src/unity.c
)
target_include_directories(
    ${FRAMEWORK} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendors/Unity/src
)

# Create test executable
add_executable(${NAME} ${TEST_SRCS})
target_include_directories(
    ${NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
    ${NAME} PRIVATE
    ${FRAMEWORK}
    ${COMMON_LIB}
    ${DEPENDENCIES}
)
set(BINDIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set_target_properties(
    ${NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BINDIR}
)

# Run tests
add_test(NAME ${NAME} COMMAND ${NAME})