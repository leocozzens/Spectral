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

    WinTools mainWinToolset = { display_cycle, display_setup, display_cleanup }; // TODO: Make window implementation threaded
    void *mainWin = interface_window_init(screenDim, WINDOW_NAME, &mainWinToolset, NULL, &errVal);
    ERR_OUT(mainWin == NULL, errVal);

    int exitVal = interface_window_run(mainWin);
    interface_cleanup();
    return exitVal;
}