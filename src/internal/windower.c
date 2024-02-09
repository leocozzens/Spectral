// C standard headers
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
// POSIX headers
#include <pthread.h>
// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Local headers
#include <Spectral/result.h>
#include <dimension.h>
#include <windower.h>
#include <d_array.h>

#define WINDOW_LIST_SIZE    2

typedef struct {
    GLFWwindow *window;
    // WinTools toolSet;
    WindowDraw cycle_win;
    pthread_t thread;
    pthread_mutex_t threadLock;
    pthread_cond_t threadSignal;
    Result drawRes;
    void *data;
    bool resizing;
} WindowHandle;

static WindowHandle *get_empty_handle(void);

static void display_runtime_error(int code, const char *msg);
static void gl_context_init(void *window);
static void viewport_size_adjust(GLFWwindow *win, int x, int y);
static void *window_worker(void *data);
static bool any_exist(void);
static void clear_handle(WindowHandle *targetHandle);
static void close_window(WindowHandle *targetHandle);

static WindowHandle *handleList = NULL;

// Public functions
Result windower_init(void) {
    if(glfwInit() == GL_FALSE)
        return create_error_result("Failed to initialize GLFW");

    handleList = D_ARRAY_CREATE(WindowHandle, WINDOW_LIST_SIZE);
    if(handleList == NULL) {
        glfwTerminate();
        return (Result){ ENOMEM, strerror(ENOMEM) };
    }
    glfwSetErrorCallback(display_runtime_error);
    return create_success_result("Successfully initialized GLFW");
}

void windower_cleanup(void) {
    for(size_t i = 0; i < D_ARRAY_LEN(handleList); i++) {
        if(handleList[i].window == NULL) continue;
        glfwSetWindowShouldClose(handleList[i].window, true);
        close_window(handleList + i);
    }
    d_destroy_array((void**) &handleList);
    glfwTerminate();
}

Result window_create(Dimension windowDim, const char *windowName, WinTools toolSet, void *data) {
    WindowHandle *activeHandle = get_empty_handle();
    if(activeHandle == NULL)
        return (Result){ ENOMEM, strerror(ENOMEM) }; 
    
    activeHandle->window = glfwCreateWindow(windowDim.width, windowDim.height, windowName, NULL, NULL);
    if(activeHandle->window == NULL)
        return create_error_result("Failed to create window frame");

    activeHandle->cycle_win = toolSet.cycle_win;
    activeHandle->threadLock = PTHREAD_MUTEX_INITIALIZER;
    activeHandle->threadSignal = PTHREAD_COND_INITIALIZER;
    
    activeHandle->data = data;
    activeHandle->drawRes = WINDOW_CLOSE_SUCCESS;
    activeHandle->resizing = false;
    gl_context_init(activeHandle->window);
    glfwSetFramebufferSizeCallback(activeHandle->window, viewport_size_adjust);

    int threadStatus = pthread_create(&activeHandle->thread, NULL, window_worker, activeHandle);
    if(threadStatus != 0) {
        glfwDestroyWindow(activeHandle->window);
        clear_handle(activeHandle);
        return create_result(threadStatus, "Failed to create rendering thread");
    }
    return create_success_result("Succesfully created and initialized window");
}

void windower_loop(void) {
    while(any_exist()) {
        glfwWaitEvents();
        for(size_t i = 0; i < D_ARRAY_LEN(handleList); i++) {
            if(handleList[i].window == NULL) continue;
            if(glfwWindowShouldClose(handleList[i].window))
                close_window(handleList + i);
        }
    }
}

// void windower_destroy(void)

// Internal functions
static void display_runtime_error(int code, const char *msg) {
    fprintf(stderr, "RUNTIME ERROR CODE %d: %s\n", code, msg);
}

static void display_windower_error(Result errorData) {
    if(errorData.code != 0)
        fprintf(stderr, "WINDOW ERROR CODE %d: %s\n", errorData.code, errorData.msg);
}

static WindowHandle *get_empty_handle(void) {
    for(size_t i = 0; i < D_ARRAY_LEN(handleList); i++) {
        if(handleList[i].window == NULL) 
            return handleList + i;
    }
    WindowHandle *tmpList = d_check_array(handleList, 1, sizeof(WindowHandle));
    if(tmpList == NULL)
        return NULL;
    handleList = tmpList;

    return &D_ARRAY_NEXT(handleList);
}

static void gl_context_init(void *window) {
    GLFWwindow *prev = glfwGetCurrentContext();
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwMakeContextCurrent(prev);
}

static void viewport_size_adjust(GLFWwindow *window, int x, int y) {
    WindowHandle *targetHandle = NULL;
    for(size_t i = 0; i < D_ARRAY_LEN(handleList); i++) {
        if(window == handleList[i].window)
            targetHandle = handleList + i;
    }
    if(targetHandle->window == NULL) return;

    targetHandle->resizing = true;
    pthread_mutex_lock(&targetHandle->threadLock);
    GLFWwindow *prev = glfwGetCurrentContext();
    
    glfwMakeContextCurrent(targetHandle->window);
    glViewport(0, 0, x, y);
    targetHandle->drawRes = targetHandle->cycle_win(targetHandle->window, targetHandle->data);
    glfwSwapBuffers(targetHandle->window);
    
    glfwMakeContextCurrent(prev); 
    targetHandle->resizing = false;
    pthread_cond_signal(&targetHandle->threadSignal);
    pthread_mutex_unlock(&targetHandle->threadLock);
}

static void *window_worker(void *handlePtr) {
    WindowHandle *handle = handlePtr;
    pthread_mutex_lock(&handle->threadLock);
    glfwMakeContextCurrent(handle->window);

    while(!glfwWindowShouldClose(handle->window) && handle->drawRes.code == 0) {
        if(handle->resizing) {
            glfwMakeContextCurrent(NULL);
            pthread_cond_wait(&handle->threadSignal, &handle->threadLock);
            glfwMakeContextCurrent(handle->window);
        }

        handle->drawRes = handle->cycle_win(handle->window, handle->data);
        glfwSwapBuffers(handle->window);
    }
    glfwSetWindowShouldClose(handle->window, true);
    pthread_mutex_unlock(&handle->threadLock);

    return NULL;
}

static bool any_exist(void) {
    for(size_t i = 0; i < D_ARRAY_LEN(handleList); i++) {
        if(handleList[i].window == NULL) continue;
        if(!glfwWindowShouldClose(handleList[i].window))
            return true;
    }
    return false;
}

static void clear_handle(WindowHandle *targetHandle) {
    pthread_mutex_destroy(&targetHandle->threadLock);
    pthread_cond_destroy(&targetHandle->threadSignal);
    targetHandle->window = NULL;
}

static void close_window(WindowHandle *targetHandle) {
    Result *windowRes;
    pthread_join(targetHandle->thread, NULL);
    display_windower_error(targetHandle->drawRes);

    glfwDestroyWindow(targetHandle->window);
    clear_handle(targetHandle);
}