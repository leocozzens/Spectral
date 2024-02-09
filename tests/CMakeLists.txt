cmake_minimum_required(VERSION 3.27.9)

# Sources and test naming
set(TEST_EXEC "run_tests")
set(TEST_SRCS test_main.c test_errors.c example.c)
list(TRANSFORM TEST_SRCS PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/)

set(TEST_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(FRAMEWORK Unity)

# Assemble the testing framework
GetRepo(
    NAME        ${FRAMEWORK}
    SOURCES     src/unity.c
    INCLUDES    src
)

# Create test executable
add_executable(${TEST_EXEC} ${TEST_SRCS})
target_include_directories(
    ${TEST_EXEC} PRIVATE 
    ${TEST_INCLUDES}
    ${INTERNAL_INCLUDES}
)
target_link_libraries(
    ${TEST_EXEC} PRIVATE
    ${FRAMEWORK}
    ${PROJECT_NAME}
)
set_target_properties(
    ${NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
)

# Run tests
function(run_test TEST_ID)
    add_test(NAME ${TEST_ID} COMMAND ${TEST_EXEC} ${TEST_ID})
endfunction()

run_test(testExample)
run_test(testExample1)