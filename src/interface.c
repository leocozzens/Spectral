// C standard headers
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Local headers
#include <interface.h>
#include <interface_errors.h>
#include <raw.h>

#define EXIT_FAILURE                                    1
#define EXIT_SUCCESS                                    0
#define GROWTH_FACTOR                                   2
#define ERR_OUT(_check, _errVal, _errStr)               if(_check) { \
                                                            *_errVal = _errStr; \
                                                            return true; \
                                                        }
#define SET_CALLBACK(_callback, _set)                   if(_callback != NULL) _set(_callback);
#define SET_WINDOW_CALLBACK(_window, _callback, _set)   if(_callback != NULL) _set(_window, _callback);

typedef struct {
    GLFWwindow *window;
    WinTools toolset;
    int exitVal;
} WinInfo;

typedef struct {
    WinInfo *handles;
    unsigned int activeCount;
    unsigned int capacity;
    unsigned int growthFactor;
} WinHandles;

static WinHandles handleSet;

// Local function prototypes
static bool init_handle_set(unsigned int initSize, unsigned int growthFactor);
static bool create_window_handle(GLFWwindow *targetWindow, WinTools *winToolset);
static WinInfo *get_window_handle(GLFWwindow *targetWindow);
static void remove_window_handle(GLFWwindow *targetWindow);
static void kill_handle_set(void);

static void viewport_size_adjust(GLFWwindow *window, int w, int h);
static bool get_mon_dim(GLFWmonitor *targetMonitor, Dimension *screenDim, char **errVal);
static void run_refresh(GLFWwindow *window);

// Public functions
char *interface_init(ErrorCallback error_callback) {
    SET_CALLBACK(error_callback, glfwSetErrorCallback);
    
    if(glfwInit() == GL_FALSE) return INTERFACE_BAD_GLFW_INIT;
    if(init_handle_set(1, GROWTH_FACTOR)) return INTERFACE_BAD_MEM_INIT;
    raw_set_gl_ver();
    return NULL;
}

bool interface_get_dim(Dimension *screenDim, char **errVal) {
    char *errFallback;
    if(errVal == NULL) errVal = &errFallback;
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    ERR_OUT(primaryMonitor == NULL, errVal, INTERFACE_NO_MON);
    return get_mon_dim(primaryMonitor, screenDim, errVal);
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

void *interface_window_init(Dimension size, char *windowName, WinTools *winToolset, WinCallbacks *callbacks, char **errVal) {
    char *errFallback;
    if(errVal == NULL) errVal = &errFallback;
    if(size.width == 0 || size.height == 0 || windowName == NULL || winToolset == NULL || winToolset->cycle_win == NULL) {
        *errVal = INTERFACE_BAD_WINDOW_INPUT;
        return NULL;
    }
    GLFWwindow* newWindow = glfwCreateWindow(size.width, size.height, windowName, NULL, NULL);
    if (newWindow == NULL) { *errVal = INTERFACE_BAD_GLFW_WIN; return NULL; }
    glfwMakeContextCurrent(newWindow);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    if(create_window_handle(newWindow, winToolset) || (winToolset->setup_win != NULL && winToolset->setup_win(newWindow))) {
        glfwDestroyWindow(newWindow);
        kill_handle_set();
        *errVal = INTERFACE_WIN_SETUP_FAIL;
        return NULL;
    }
    glViewport(0, 0, size.width, size.height);
    glfwSetWindowRefreshCallback(newWindow, run_refresh);
    glfwSetFramebufferSizeCallback(newWindow, viewport_size_adjust);

    if(callbacks == NULL) return newWindow;
    SET_WINDOW_CALLBACK(newWindow, callbacks->key_func, glfwSetKeyCallback);
    SET_WINDOW_CALLBACK(newWindow, callbacks->char_func, glfwSetCharCallback);
    SET_WINDOW_CALLBACK(newWindow, callbacks->size_func, glfwSetWindowSizeCallback);
    SET_WINDOW_CALLBACK(newWindow, callbacks->cursor_pos_func, glfwSetCursorPosCallback);
    SET_WINDOW_CALLBACK(newWindow, callbacks->cursor_enter_func, glfwSetCursorEnterCallback);
    SET_WINDOW_CALLBACK(newWindow, callbacks->scroll_func, glfwSetScrollCallback);
    SET_WINDOW_CALLBACK(newWindow, callbacks->close_func, glfwSetWindowCloseCallback);
    return newWindow;
}

int interface_window_run(void *window) {
    assert(window != NULL);
    WinInfo *activeHandle = get_window_handle(window);
    if(activeHandle == NULL) return EXIT_FAILURE;

    while(!(glfwWindowShouldClose(window) || activeHandle->toolset.cycle_win(window, &(activeHandle->exitVal)))) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    if(activeHandle->toolset.cleanup_win != NULL) activeHandle->toolset.cleanup_win(window);
    remove_window_handle(window);
    return activeHandle->exitVal;
}

void interface_cleanup(void) {
    glfwTerminate();
    kill_handle_set();
}

// Local function definitions
static bool init_handle_set(unsigned int initSize, unsigned int growthFactor) {
    handleSet.handles = malloc(sizeof(WinInfo) * initSize);
    if(handleSet.handles == NULL) return true;
    
    handleSet.activeCount = 0;
    handleSet.capacity = initSize;
    handleSet.growthFactor = growthFactor;
    return false;
}

static bool create_window_handle(GLFWwindow *targetWindow, WinTools *winToolset) {
    if(targetWindow == NULL || winToolset == NULL) return true;
    unsigned int setIndex = 0;

    if(handleSet.activeCount >= handleSet.capacity) {
        WinInfo *tmp = realloc(handleSet.handles, sizeof(WinInfo) * (handleSet.capacity * handleSet.growthFactor));
        if(tmp == NULL) return true;
        handleSet.handles = tmp;
        handleSet.capacity *= handleSet.growthFactor;
        setIndex = handleSet.activeCount;
    }
    for(; setIndex < handleSet.activeCount; setIndex++) {
        if(handleSet.handles[setIndex].window == NULL) break;
    }

    handleSet.handles[setIndex].window = targetWindow;
    handleSet.handles[setIndex].toolset = *winToolset;
    handleSet.handles[setIndex].exitVal = EXIT_SUCCESS;

    handleSet.activeCount++;
    return false;
}

static WinInfo *get_window_handle(GLFWwindow *targetWindow) {
    unsigned int readMax = handleSet.activeCount;
    for(unsigned int i = 0; i < readMax; i++) {
        if(handleSet.handles[i].window == NULL) { readMax++; continue; }

        if(handleSet.handles[i].window == targetWindow) return handleSet.handles + i;
    }
    return NULL;
}

static void remove_window_handle(GLFWwindow *targetWindow) {
    unsigned int readMax = handleSet.activeCount;
    for(unsigned int i = 0; i < readMax; i++) {
        if(handleSet.handles[i].window == NULL) { readMax++; continue; }

        if(handleSet.handles[i].window == targetWindow) {
            handleSet.handles[i].window = NULL;
            handleSet.activeCount--;
        }
    }
}

static void kill_handle_set(void) {
    free(handleSet.handles);
    handleSet.handles = NULL;
    handleSet.activeCount = 0;
    handleSet.capacity = 0;
}

static void viewport_size_adjust(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
}

static bool get_mon_dim(GLFWmonitor *targetMonitor, Dimension *screenDim, char **errVal) {
    assert(targetMonitor != NULL);
    char *errFallback;
    if(errVal == NULL) errVal = &errFallback;

    const GLFWvidmode *mainMode;
    mainMode = glfwGetVideoMode(targetMonitor);

    ERR_OUT(mainMode == NULL, errVal, INTERFACE_NO_MON);
    screenDim->width = mainMode->width;
    screenDim->height = mainMode->height;
    ERR_OUT(screenDim->width == 0 || screenDim->height == 0, errVal, INTERFACE_INVALID_SIZE);

    return false;
}

static void run_refresh(GLFWwindow *window) {
    WinInfo *windowHandle = get_window_handle(window);
    if(windowHandle == NULL) return;
    windowHandle->toolset.cycle_win(window, &(windowHandle->exitVal));
    glfwSwapBuffers(window);
}