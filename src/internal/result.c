// Standard C headers
#include <string.h>
// Local headers
#include <Spectral/result.h>

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1

#define STANDARD_SUCCESS_MSG    "Operation executed successfully"
#define STANDARD_ERROR_MSG      "Operation failed"

// Standard results
const Result STANDARD_SUCCESS = SUCCESS_RESULT(STANDARD_SUCCESS_MSG);
const Result STANDARD_FAILURE = ERROR_RESULT(STANDARD_ERROR_MSG);

// Window results
const Result WINDOW_CLOSE_SUCCESS = SUCCESS_RESULT("Window closed gracefully");

Result create_result(int code, const char *msg) {
    if(msg == NULL) {
        if(code == EXIT_SUCCESS) msg = STANDARD_SUCCESS_MSG;
        else msg = strerror(code);
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