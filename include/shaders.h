#ifndef SHADERS_H
#define SHADERS_H

typedef unsigned int GLuint;

typedef void (*ErrorCallback)(int errorCode, const char *description);

char *shader_concatenate_version(const char *shaderData);
GLuint shader_load(const char *vertexShader, const char *fragmentShader, ErrorCallback display_error);

#endif