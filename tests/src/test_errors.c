// Testing suite
#include <unity.h>
// Local test headers
#include <test_errors.h>

void unknownTest(void) {
    UNITY_TEST_FAIL(__LINE__, UNKNOWN_MSG);
}

void badUsage(void) {
    UNITY_TEST_FAIL(__LINE__, BAD_USAGE_MSG);
}