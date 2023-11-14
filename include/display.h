#ifndef DISPLAY_H
#define DISPLAY_H

void display_runtime_error(int errorCode, const char *description);
bool display_setup(void *window);
bool display_cycle(void *window, int *exitVal);
void display_cleanup(void *window);

#endif