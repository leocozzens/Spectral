// C standard headers
#include <stdio.h>
// Local headers
#include <interface.h>
#include <display.h>

#define EXIT_FAILURE            1
#define EXIT_SUCCESS            0
#define Y_PORTION               0.5F
#define X_SCALE                 2.0F

#define FATAL_ERR_FMT           "FATAL ERROR: %s\nExiting program\n"
#define WINDOW_NAME             "Test window"

#define ERR_OUT(_cond, _errVal) if(_cond) { \
                                    fprintf(stderr, FATAL_ERR_FMT, _errVal); \
                                    return EXIT_FAILURE; \
                                }
static char *errVal = NULL;

int main(void) {
    errVal = interface_init(display_runtime_error);
    ERR_OUT(errVal != NULL, errVal);

    Dimension screenDim;
    ERR_OUT(interface_get_dim(&screenDim, &errVal), errVal);
    interface_cut_dim(&screenDim, Y_PORTION, X_SCALE, false);
    
    void *primaryWin = window_init(screenDim, WINDOW_NAME, display_setup, &errVal);
    ERR_OUT(primaryWin == NULL, errVal);

    int exitVal = window_run(primaryWin, display_cycle);

    display_cleanup();
    interface_cleanup();
    return exitVal;
}