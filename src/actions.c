// C standard headers
#include <stdio.h>
#include <stdbool.h>
// External libraries
#include <GLFW/glfw3.h>

#define SOFT_BLUE   .20, .20, .60, .6

// static void display_error_message(int errorCode, const char *description) {

// }

void action_glfw_error(int errorCode, const char *description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

bool action_win_cycle(void *window, int *exitVal) {
    glClearColor(SOFT_BLUE);
    glClear(GL_COLOR_BUFFER_BIT);
    return false;
}