// C standard headers
#include <assert.h>
#include <math.h>
// External libraries
#include <GLFW/glfw3.h>

#ifndef OPENGL_VER
#define OPENGL_VER  3.3
#endif

#ifndef GL_PROFILE
#define GL_PROFILE  GLFW_OPENGL_COMPAT_PROFILE
#endif

static int rawGlVer[2];
static int rawGlProfile;
void raw_set_gl_ver(void) {
    float integerSegment;
    float fractionSegment = modff(OPENGL_VER, &integerSegment);
    assert(fractionSegment != NAN && integerSegment != INFINITY);

    rawGlVer[0] = integerSegment;
    rawGlVer[1] = ceil(fractionSegment * 10);
    rawGlProfile = GL_PROFILE;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, rawGlVer[0] );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, rawGlVer[1] );
    glfwWindowHint(GLFW_OPENGL_PROFILE, GL_PROFILE);
}

int *raw_get_gl_ver(void) {
    return rawGlVer;
}

int raw_get_gl_profile(void) {
    return rawGlProfile;
}