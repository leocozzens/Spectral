// C standard headers
#include <stdio.h>
// Local headers
#include <result.h>
#include <dimension.h>
#include <windower.h>
#include <renderer.h>

#define FATAL_ERR_FMT                   "FATAL ERROR: %s\nExiting program\n"
#define CHECK_RESULT(_result, _actions) if(_result.code != 0) { \
                                            fprintf(stderr, FATAL_ERR_FMT, _result.msg); \
                                            _actions; \
                                            return _result.code; \
                                        }
                                        
int main(void) {
    Result retData = STANDARD_SUCCESS;
    retData = windower_init();
    CHECK_RESULT(retData,);

    retData = window_create((Dimension){ 600, 480 }, "TEST ONE", renderer_draw, NULL);
    CHECK_RESULT(retData, windower_cleanup());
    retData = window_create((Dimension){ 600, 480 }, "TEST TWO", renderer_draw, NULL);
    CHECK_RESULT(retData, windower_cleanup());
    
    windower_loop();

    return 0;
}