// C standard headers
#include <stdbool.h>
#include <assert.h>
// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Local headers
#include <interface.h>
#include <interface_errors.h>
#include <raw.h>

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

static void glfw_framebuffer_size_adjust(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
}

char *interface_init(void (*glfw_error)(int, const char*)) { // TODO: Add structure that allows you to define pointer for any callback
    glfwSetErrorCallback(glfw_error);
    if(glfwInit() == GL_FALSE) return INTERFACE_BAD_GLFW_INIT;

    raw_set_gl_ver();
    return NULL;
}

bool interface_get_dim(Dimension *screenDim, char **errVal) {
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    ERR_OUT(primaryMonitor == NULL, errVal, INTERFACE_NO_MON);
    return get_dim(primaryMonitor, screenDim, errVal);
}

void interface_cut_dim(Dimension *targetDim, float portion, float scale, bool xScaled) {
    int *primary = &targetDim->height;
    int *secondary = &targetDim->width;
    if(xScaled) {
        primary = &targetDim->width;
        secondary = &targetDim->height;
    }
    *primary *= portion;
    *secondary = *primary * scale;
}

void *window_init(Dimension size, char *windowName, WinSetup win_setup, char **errVal) {
    if(size.width == 0 || size.height == 0 || windowName == NULL || errVal == NULL) return NULL;
    GLFWwindow* newWindow = glfwCreateWindow(size.width, size.height, windowName, NULL, NULL);

    if (newWindow == NULL) {
        *errVal = INTERFACE_BAD_GLFW_WIN;
        return NULL;
    }
    glfwMakeContextCurrent(newWindow);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    if(win_setup != NULL && win_setup(newWindow)) {
        glfwDestroyWindow(newWindow);
        *errVal = INTERFACE_WIN_SETUP_FAIL;
        return NULL;
    }
    glViewport(0, 0, size.width, size.height);
    glfwSetFramebufferSizeCallback(newWindow, glfw_framebuffer_size_adjust);
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