// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Local headers
#include <dimension.h>
#include <Spectral/result.h>
#include <colors.h>

#define POSITION_ELEM                           2
#define COLOR_ELEM                              4

typedef struct {
    union {
        float pos[POSITION_ELEM];
        struct {
            float x;
            float y;
        };
    };
    union {
        float color[COLOR_ELEM];
        struct {
            float r;
            float g;
            float b;
            float a;
        };
    };
} Vertex;

typedef struct {
    unsigned int VAO, numElems;
} DrawDetails;

Result renderer_draw(void *window, void *data) {
    glClearColor(SLATE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return WINDOW_CLOSE_SUCCESS;
}