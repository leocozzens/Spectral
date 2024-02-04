// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Local headers
#include <dimension.h>

#define SLATE 0.10F, 0.10F, 0.10F, 1.00F

void renderer_draw(void *data) {
    glClearColor(SLATE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}