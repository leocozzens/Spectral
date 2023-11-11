// C standard headers
#include <stdio.h>
#include <stdbool.h>
// External libraries
#include <GLFW/glfw3.h>
// Local headers
#include <data.h>
#include <render.h>

#define RUNTIME_ERR_FMT "RUNTIME ERROR - %s\n"

#define PURPLE          0.20F, 0.20F, 0.60F, 1.00F
#define DARK_GREY       0.20F, 0.20F, 0.20F, 1.00F

static bool display_error_box(int errorCode, const char *description) {
    return true;
}

void display_runtime_error(int errorCode, const char *description) {
    if(display_error_box(errorCode, description))
        fprintf(stderr, RUNTIME_ERR_FMT, description);
}

static DrawList dList;

bool display_setup(void *window) {
    glClearColor(PURPLE);

    Vertex vertices[] = {
        {{ 0.0F, 0.5F, 0.0F }}, // Top corner
        {{ -0.5F, -0.5F, 0.0F }}, // Bottom left corner
        {{ 0.5F, -0.5F, 0.0 }}
    };
    unsigned int elems[] = { 0, 1, 2 };

    VertexList vList = { vertices, sizeof(vertices) / sizeof(Vertex), 0 };
    ElemList eList = { elems, sizeof(elems) / sizeof(unsigned int), 0 };

    INIT_LIST(dList, 1, sizeof(DrawDetails));
    if(dList.list == NULL) return true;

    DrawDetails newDetails = render_establish_mesh(vList, eList);
    if(ADDTO_LIST(dList, newDetails)) {
        KILL_LIST(dList);
        return true;
    }
    return false;
}

bool display_cycle(void *window, int *exitVal) {
    glClear(GL_COLOR_BUFFER_BIT);
    render_draw(dList);
    return false;
}

void display_cleanup(void) {
    render_delete_mesh(dList);
    KILL_LIST(dList);
}