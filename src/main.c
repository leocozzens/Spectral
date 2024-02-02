// C standard headers
#include <stdio.h>
// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

    GLFWwindow *master;
    retData = window_create((void**) &master, create_dimension(600, 480), "Test window 123");
    CHECK_RESULT(retData, glfwTerminate());

    while(!glfwWindowShouldClose(master)) {
        glfwPollEvents();
        renderer_draw(master);
    }
    glfwDestroyWindow(master);
    glfwTerminate();
    return 0;
}
