#include <unity.h>
#include <test.h>

void setUp(void) {
    // set up stuff here
}

void tearDown(void) {
    // clean stuff up here
}

void testExample(void) {
    int a = 1;
    TEST_ASSERT(a == 1); //this one will pass
    TEST_ASSERT(a == 2); //this one will fail
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testExample);
    return UNITY_END();
}