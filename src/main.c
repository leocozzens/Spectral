// C standard headers
#include <stdio.h>
// GL loader
#include <glad/glad.h>
// Windowing API
#include <GLFW/glfw3.h>
// Local headers
#include <renderer.h>

void testErr(int a, const char *err) {
    fprintf(stderr, "GLFW ERR: %s\n", err);
}

static void viewport_size_adjust(GLFWwindow *win, int x, int y) {
    glViewport(0, 0, x, y);
    renderer_draw(win);
}

int main(void) {
    if(glfwInit() == GL_FALSE) {
        glfwTerminate();
        return 1;
    }

    glfwSetErrorCallback(testErr);
    GLFWwindow *master = glfwCreateWindow(600, 480, "Test", NULL, NULL);
    if(master == NULL) {
        glfwTerminate();
        fprintf(stderr, "ERROR: Window failed\n");
        return 1;
    }
    glfwMakeContextCurrent(master);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(master, viewport_size_adjust);
    glViewport(0, 0, 600, 480);
    while(!glfwWindowShouldClose(master)) {
        glfwPollEvents();
        renderer_draw(master);
    }
    glfwDestroyWindow(master);
    glfwTerminate();
    return 0;
}
