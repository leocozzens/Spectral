// C standard headers
#include <stdbool.h>
// Testing suite
#include <unity.h>
// Project headers
// #include <test.h>
// Local test headers
#include <test_errors.h>
#include <example.h>

#define STRINGIZE(_x)                   #_x
#define CHECK_AND_RUN(_target, _test)   if(string_compare(_target, STRINGIZE(_test))) RUN_TEST(_test)
#define DEFAULT_NAME                    "run_tests"

void setUp(void) {
    // set up stuff here
}

void tearDown(void) {
    // clean stuff up here
}

static bool string_compare(const char *s1, const char *s2);
static char *get_last_chr(char *str, char target);

int main(int argc, char **argv) {
    UNITY_BEGIN();
    if(argc < 2) {
        char *name = argv[0];
        if(name == NULL) name = DEFAULT_NAME; 
        else {
            char *lastSlash = get_last_chr(name, '\\');
            if(lastSlash == NULL) lastSlash = get_last_chr(name, '/');
            if(lastSlash != NULL) name = lastSlash + 1;
        }
        fprintf(stderr, "Error: %s\nUsage: %s [TESTS..]\n\n", BAD_USAGE_MSG, name); 
        RUN_TEST(badUsage);
        return UNITY_END();
    }
    for(int i = 1; i < argc; i++) {
        CHECK_AND_RUN(argv[i], testExample);
        else CHECK_AND_RUN(argv[i], testExample1);
        else RUN_TEST(unknownTest);
    }
    return UNITY_END();
}

static bool string_compare(const char *s1, const char *s2) {
    do {
        if(*s1 != *s2++) return false;
    } while(*s1++ != '\0');
    return true;
}

static char *get_last_chr(char *str, char target) {
    char *chr = NULL;
    do {
        if(*str == target) chr = str;
        str++;
    } while(*str != '\0');
    return chr;
}