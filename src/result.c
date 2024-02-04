// Local headers
#include <result.h>

#define NULL            ((void*)0)
#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1

#define STANDARD_SUCCESS_MSG    "Operation executed successfully\n"
#define STANDARD_ERROR_MSG      "Operation failed\n"

const Result STANDARD_SUCCESS = { EXIT_SUCCESS, STANDARD_SUCCESS_MSG };
const Result STANDARD_FAILURE = { EXIT_FAILURE, STANDARD_ERROR_MSG };

Result create_result(int code, const char *msg) {
    if(msg == NULL) {
        if(code == 0) msg = STANDARD_SUCCESS_MSG;
        else msg = STANDARD_ERROR_MSG;
    }
    Result newResult = { code, msg };
    return newResult;
}

Result create_success_result(const char *msg) {
    if(msg == NULL) return STANDARD_SUCCESS;
    Result newResult = { EXIT_SUCCESS, msg };
    return newResult;
}

Result create_error_result(const char *msg) {
    if(msg == NULL) return STANDARD_FAILURE;
    Result newResult = { EXIT_FAILURE, msg };
    return newResult;
}