#ifndef ERR_DATA_H
#define ERR_DATA_H

typedef struct Result {
    int code;
    const char *msg;
} Result;

Result create_result(int retCode, const char *msg);
Result create_success_result(const char *msg);
Result create_error_result(const char *msg);

extern const Result STANDARD_SUCCESS;
extern const Result STANDARD_FAILURE;
#endif