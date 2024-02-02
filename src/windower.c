// C standard headers
#include <stdio.h>
// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Local headers
#include <result.h>
#include <dimension.h>
#include <renderer.h>

static void gl_context_init(void *window);
static void viewport_size_adjust(GLFWwindow *win, int x, int y);
static void display_runtime_error(int code, const char *msg);

// Public functions
Result windower_init(void) {
    if(glfwInit() == GL_FALSE)
        return create_error_result("Failed to initialize GLFW\n");

    glfwSetErrorCallback(display_runtime_error);
    return create_success_result("Successfully initialized GLFW\n");
}

Result window_create(void **window, Dimension windowDim, const char *windowName) {
    *window = glfwCreateWindow(windowDim.width, windowDim.height, windowName, NULL, NULL);
    if(*window == NULL)
        return create_error_result("Failed to create window\n");

    gl_context_init(*window);
    glfwSetFramebufferSizeCallback(*window, viewport_size_adjust);
}

// Internal functions
static void gl_context_init(void *window) {
    GLFWwindow *prev = glfwGetCurrentContext();
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    if(prev != NULL) glfwMakeContextCurrent(prev);
}

static void viewport_size_adjust(GLFWwindow *window, int x, int y) {
    glViewport(0, 0, x, y);
    renderer_draw(window);
}

static void display_runtime_error(int code, const char *msg) {
    fprintf(stderr, "RUNTIME ERROR: %s\n", msg);
}