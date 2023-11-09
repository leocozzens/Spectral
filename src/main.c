// C standard headers
#include <stdio.h>
// Local headers
#include <interface.h>
#include <actions.h>

#define EXIT_FAILURE            1
#define EXIT_SUCCESS            0

#define FATAL_ERR_FMT           "FATAL ERROR: %s\nExiting program\n"
#define WINDOW_NAME             "Test window"


#define ERR_OUT(_cond, _errVal) if(_cond) { \
                                    fprintf(stderr, FATAL_ERR_FMT, _errVal); \
                                    return EXIT_FAILURE; \
                                }
static char *errVal = NULL;

int main(void) {
    errVal = interface_init();
    ERR_OUT(errVal != NULL, errVal);

    Dimension screenDim;
    ERR_OUT(interface_get_dim(&screenDim, &errVal), errVal);
    screenDim.height /= 2;
    screenDim.width = screenDim.height * 2;
    
    void *primaryWin = window_init(screenDim, "Test window", &errVal);
    ERR_OUT(primaryWin == NULL, errVal);

    int exitVal = window_run(primaryWin, action_win_cycle);
    interface_cleanup();
    return exitVal;
}