// C standard headers
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Local headers
#include <data.h>
#include <render.h>
#include <shaders.h>

#define RUNTIME_ERR_FMT         "RUNTIME ERROR - %s\n"
#define KILL_RET(_cond, _extra) if(_cond) { KILL_LIST(dList); _extra; return true; }

#define PURPLE          0.20F, 0.20F, 0.60F, 1.00F
#define DARK_GREY       0.20F, 0.20F, 0.20F, 1.00F
#define SLATE           0.10F, 0.10F, 0.10F, 1.00F

static bool display_error_box(int errorCode, const char *description) {
    return true;
}

void display_runtime_error(int errorCode, const char *description) {
    if(display_error_box(errorCode, description))
        fprintf(stderr, RUNTIME_ERR_FMT, description);
}

static DrawList dList;
static GLuint mainShader;

static float random_float(float max) {
    return ((float) rand() / (float) (RAND_MAX)) * max;
}

bool display_setup(void *window) {
    glClearColor(SLATE);
    srand(time(NULL));
    Vertex vertices[4] = {
        {{{ -1.0F, 1.0F }}, {{ random_float(1.0), random_float(1.0), random_float(1.0), 1.0F }}},
        {{{ -1.0F, -1.0F }}, {{ random_float(1.0), random_float(1.0), random_float(1.0), 1.0F }}},
        {{{ 1.0F, 1.0F }}, {{ random_float(1.0), random_float(1.0), random_float(1.0), 1.0F }}},
        {{{ 1.0F, -1.0F }}, {{ random_float(1.0), random_float(1.0), random_float(1.0), 1.0F }}}
    };
    unsigned int elems[] = { 0, 1, 2, 3, 0};

    VertexList vList = { vertices, sizeof(vertices) / sizeof(Vertex), 0 };
    ElemList eList = { elems, sizeof(elems) / sizeof(unsigned int), 0 };

    INIT_LIST(dList, 1, sizeof(DrawDetails));
    if(dList.list == NULL) return true;

    DrawDetails newDetails = render_establish_mesh(vList, eList);
    KILL_RET(ADDTO_LIST(dList, newDetails),);

    char *vertShader =
        #include <vertex_shader.h>
    ;
    vertShader = shader_concatenate_version(vertShader);
    KILL_RET(vertShader == NULL,);

	char *fragShader =
        #include <fragment_shader.h>
    ;
    fragShader = shader_concatenate_version(fragShader);
    KILL_RET(fragShader == NULL, free(vertShader));

    mainShader = shader_load(vertShader, fragShader, display_runtime_error);
    free(vertShader);
    free(fragShader);
    return false;
}

bool display_cycle(void *window, int *exitVal) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(mainShader);

    render_draw(dList);
    return false;
}

void display_cleanup(void *window) {
    render_delete_mesh(dList);
    KILL_LIST(dList);
}