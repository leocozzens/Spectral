// C standard headers
#include <stdbool.h>
#include <assert.h>
#include <math.h>
// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Local headers
#include <interface.h>
#include <interface_errors.h>

#define EXIT_SUCCESS                        0
#define ERR_OUT(_check, _errVal, _errStr)   if(_check) { \
                                                *_errVal = _errStr; \
                                                return true; \
                                            }

static bool get_dim(GLFWmonitor *targetMonitor, Dimension *screenDim, char **errVal) {
    assert(targetMonitor != NULL);

    const GLFWvidmode *mainMode;
    mainMode = glfwGetVideoMode(targetMonitor);

    ERR_OUT(mainMode == NULL, errVal, INTERFACE_NO_MON);
    screenDim->width = mainMode->width;
    screenDim->height = mainMode->height;
    ERR_OUT(screenDim->width == 0 || screenDim->height == 0, errVal, INTERFACE_INVALID_SIZE);

    return false;
}

char *interface_init(void (*glfw_error)(int, const char*)) {
    glfwSetErrorCallback(glfw_error);
    if (glfwInit() == GL_FALSE) return INTERFACE_BAD_GLFW_INIT;

    #ifndef OPENGL_VER
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #else
    float integerSegment;
    float fractionSegment = modff(OPENGL_VER, &integerSegment);
    assert(fractionSegment != NAN || integerSegment == INFINITY);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, (int) integerSegment);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, (int) ceil(fractionSegment * 10));
    #endif
    return NULL;
}

bool interface_get_dim(Dimension *screenDim, char **errVal) {
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    ERR_OUT(primaryMonitor == NULL, errVal, INTERFACE_NO_MON);
    return get_dim(primaryMonitor, screenDim, errVal);
}

void *window_init(Dimension size, char *windowName, char **errVal) {
    if(size.width == 0 || size.height == 0 || windowName == NULL || errVal == NULL) return NULL;
    GLFWwindow* newWindow = glfwCreateWindow(size.width, size.height, windowName, NULL, NULL);

    if (newWindow != NULL) {
        glfwMakeContextCurrent(newWindow);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    }
    else *errVal = INTERFACE_BAD_GLFW_WIN;
    return newWindow;
}

int window_run(void *window, WinFunc win_cycle) {
    assert(!(window == NULL || win_cycle == NULL));

    int exitVal = EXIT_SUCCESS;
    while(!(glfwWindowShouldClose(window) || win_cycle(window, &exitVal))) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return exitVal;
}

void interface_cleanup(void) {
    glfwTerminate();
}