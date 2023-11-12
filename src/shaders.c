// C standard headers
#include <stdio.h>
#include <stdlib.h>
// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Local headers
#include <shaders.h>
#include <raw.h>

#define GLSL_VERSION_TEMPLATE       "#version %d%d0 %s\n"
#define FMT_CHARACTERS              6
#define VERSION_DIGITS              2
#define GLSL_CORE_PROFILE           "core"
#define GLSL_COMPAT_PROFILE         "compatibility"

#define S_LEN(_literal)             (sizeof(_literal) - 1)
#define SET_ERR(_setter)            errorMsg = _setter
#define HANDLE_ERR(_msg, _callback) if(_msg != NULL) (_msg == memErr || _msg == noShaderLog || _msg == noLinkerLog) ? _callback(2, _msg) : _callback(1, _msg), free(_msg)

static char *memErr = "Insufficient memory";
static char *noShaderLog = "Unlisted OpenGL shader compilation error";
static char *noLinkerLog = "Unlisted OpenGL shader linking error";

static char *compile_shader(GLuint ShaderID, const GLchar *const *shaderData) {
    glShaderSource(ShaderID, 1, shaderData, NULL);
    glCompileShader(ShaderID);

    GLint success = 0;
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);
    if(success != GL_TRUE) {
        GLint logLen = 0;
        char *errorMsg;
        glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            errorMsg = malloc(logLen + 1);
            if(errorMsg == NULL) return memErr; 
            glGetShaderInfoLog(ShaderID, logLen, NULL, errorMsg);
            return errorMsg;
        }
        return noShaderLog;
    }
    return NULL;
}

static char *link_shaders(GLuint *ProgramID, GLuint VertexShaderID, GLuint FragmentShaderID) {
	*ProgramID = glCreateProgram();
	glAttachShader(*ProgramID, VertexShaderID);
	glAttachShader(*ProgramID, FragmentShaderID);
	glLinkProgram(*ProgramID);

    GLint success;
    glGetProgramiv(*ProgramID, GL_LINK_STATUS, &success);
    if(success != GL_TRUE) {
        GLint logLen = 0;
        char *errorMsg;
        glGetProgramiv(*ProgramID, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            errorMsg = malloc(logLen + 1);
            if(errorMsg == NULL) return memErr;
            glGetProgramInfoLog(*ProgramID, logLen, NULL, errorMsg);
            return errorMsg;
        }
        return noLinkerLog;
    }
    return NULL;
}

static void cleanup_shader(GLuint ProgramID, GLuint ShaderID) {
	glDetachShader(ProgramID, ShaderID);
	glDeleteShader(ShaderID);
}

static char *local_stpcpy(char *restrict dest, const char *restrict source) {
    if(dest == NULL || source == NULL) return NULL;
    while(*source != '\0') *dest++ = *source++;

    *dest = '\0';
    return dest;
}

static long long local_strlen(const char *source) {
    long long index = 0;
    while(*source++ != '\0') index++;
    return index;
}

char *shader_concatenate_version(const char *shaderData) {
    int profile = raw_get_gl_profile();
    char *profileData;
    switch(profile) {
        case GLFW_OPENGL_CORE_PROFILE:
            profileData = GLSL_CORE_PROFILE;
            break;
        case GLFW_OPENGL_COMPAT_PROFILE:
            profileData = GLSL_COMPAT_PROFILE;
            break;
        default:
            return NULL;
    }
    char *versionData = malloc(S_LEN(GLSL_VERSION_TEMPLATE) - FMT_CHARACTERS + VERSION_DIGITS + local_strlen(profileData) + 1);
    if(versionData == NULL) return NULL;
    int *glVer = raw_get_gl_ver();
    sprintf(versionData, GLSL_VERSION_TEMPLATE, glVer[0], glVer[1], profileData);

    char *completeShaderSource = malloc(local_strlen(versionData) + local_strlen(shaderData) + 1);
    if(completeShaderSource != NULL)
        local_stpcpy((local_stpcpy(completeShaderSource, versionData)), shaderData);

    free(versionData);
    return completeShaderSource;
}

GLuint shader_load(const char *vertexShader, const char *fragmentShader, ErrorCallback display_error) {
    char *errorMsg;
    GLuint ProgramID;
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    SET_ERR(compile_shader(VertexShaderID, &vertexShader));
    HANDLE_ERR(errorMsg, display_error);

    SET_ERR(compile_shader(FragmentShaderID, &fragmentShader));
    HANDLE_ERR(errorMsg, display_error);

    SET_ERR(link_shaders(&ProgramID, VertexShaderID, FragmentShaderID));
    HANDLE_ERR(errorMsg, display_error);

    cleanup_shader(ProgramID, VertexShaderID);
    cleanup_shader(ProgramID, FragmentShaderID);

    return ProgramID;
}