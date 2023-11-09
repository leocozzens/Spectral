// C standard headers
#include <stdbool.h>
// External libraries
#include <GLFW/glfw3.h>

bool action_win_cycle(void *window, int *exitVal) {
    glClear(GL_COLOR_BUFFER_BIT);

    return false;
}