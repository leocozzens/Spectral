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
#include <result.h>
#include <dimension.h>
#include <windower.h>
#include <d_array.h>

#define WINDOW_LIST_SIZE    2

static void display_runtime_error(int code, const char *msg);
static void gl_context_init(void *window);
static void viewport_size_adjust(GLFWwindow *win, int x, int y);
static void *window_worker(void *data);
static bool any_exist(void);
static void close_window(size_t index);

typedef struct {
    GLFWwindow *window;
    WindowMain winMain;
    Dimension winDim;
    pthread_t thread;
    pthread_mutex_t threadLock;
    pthread_cond_t threadSignal;
    void *data;
    bool resizing;
} WindowHandle;

static WindowHandle **handleList = NULL;

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
    d_destroy_array((void**) &handleList);
    glfwTerminate();
}

Result window_create(Dimension windowDim, const char *windowName, WindowMain winMain, void *data) {
    {
        WindowHandle **tmpList = d_check_array(handleList, 1, sizeof(WindowHandle));
        if(tmpList == NULL)
            return (Result){ ENOMEM, strerror(ENOMEM) };
        handleList = tmpList;
    }
    WindowHandle **activeHandle = &D_ARRAY_NEXT(handleList);
    *activeHandle = malloc(sizeof(WindowHandle));
    if(*activeHandle == NULL) {
        D_POP_BACK(handleList);
        return (Result){ ENOMEM, strerror(ENOMEM) }; 
    }
    
    (*activeHandle)->window = glfwCreateWindow(windowDim.width, windowDim.height, windowName, NULL, NULL);
    if((*activeHandle)->window == NULL) {
        free(*activeHandle);
        D_POP_BACK(handleList);
        return create_error_result("Failed to create window");
    }

    (*activeHandle)->data = data;
    (*activeHandle)->winMain = winMain;
    (*activeHandle)->threadLock = PTHREAD_MUTEX_INITIALIZER;
    (*activeHandle)->threadSignal = PTHREAD_COND_INITIALIZER;
    (*activeHandle)->resizing = false;
    gl_context_init((*activeHandle)->window);
    glfwSetFramebufferSizeCallback((*activeHandle)->window, viewport_size_adjust);

    int threadStatus = pthread_create(&(*activeHandle)->thread, NULL, window_worker, *activeHandle);
    if(threadStatus != 0) {
        glfwDestroyWindow((*activeHandle)->window);
        free(*activeHandle);
        D_POP_BACK(handleList);
        return (Result){ threadStatus, strerror(threadStatus) };
    }
    return create_success_result("Succesfully created and initialized window");
}

Result windower_loop(void) {
    while(any_exist()) {
        glfwPollEvents();
        for(size_t i = 0; i < D_ARRAY_LEN(handleList); i++) {
            if(glfwWindowShouldClose(handleList[i]->window))
                close_window(i);
        }
    }
    return create_success_result("Loop ended\nExiting program...");
}

// Internal functions
static void display_runtime_error(int code, const char *msg) {
    fprintf(stderr, "RUNTIME ERROR: %s\n", msg);
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
        if(window == handleList[i]->window)
            targetHandle = handleList[i];
    }
    if(targetHandle == NULL) return;

    targetHandle->resizing = true;
    pthread_mutex_lock(&targetHandle->threadLock);
    GLFWwindow *prev = glfwGetCurrentContext();
    
    glfwMakeContextCurrent(targetHandle->window);
    glViewport(0, 0, x, y);
    targetHandle->winMain(targetHandle->data);
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
    while(!glfwWindowShouldClose(handle->window)) {
        if(handle->resizing) {
            glfwMakeContextCurrent(NULL);
            pthread_cond_wait(&handle->threadSignal, &handle->threadLock);
            glfwMakeContextCurrent(handle->window);
        }

        handle->winMain(handle->data);
        glfwSwapBuffers(handle->window);
    }
    pthread_mutex_unlock(&handle->threadLock);
    return NULL;
}

static bool any_exist(void) {
    for(size_t i = 0; i < D_ARRAY_LEN(handleList); i++) {
        if(!glfwWindowShouldClose(handleList[i]->window))
            return true;
    }
    return false;
}

static void close_window(size_t index) {
    pthread_join(handleList[index]->thread, NULL);
    glfwDestroyWindow(handleList[index]->window);
    pthread_mutex_destroy(&handleList[index]->threadLock);
    pthread_cond_destroy(&handleList[index]->threadSignal);
    free(handleList[index]);
    D_ARRAY_REMOVE(handleList, index);
}