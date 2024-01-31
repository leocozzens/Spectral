#ifndef INTERFACE_ERRORS_H
#define INTERFACE_ERRORS_H

#define INTERFACE_BAD_MEM_INIT      "Failed to allocate memory needed to initialize interface"

#define INTERFACE_BAD_GLFW_INIT     "Failed to initialize GLFW"
#define INTERFACE_BAD_GLFW_WIN      "Failed to create GLFW window"

#define INTERFACE_NO_MON            "Unable to find suitable monitor"
#define INTERFACE_NO_VID            "Failed to retrieve video data for relevant monitor"
#define INTERFACE_INVALID_SIZE      "Invalid screen size for relevant monitor"

#define INTERFACE_WIN_SETUP_FAIL    "Window setup failed"
#define INTERFACE_BAD_WINDOW_CREATE "Improper window creation parameters"
#define INTERFACE_BAD_WINDOW_INIT   "Improper window initialization parameters"

#define INTERFACE_BAD_GL_CONTEXT    "Failed to initialize OpenGL context"

#endif