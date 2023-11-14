layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec4 vertexColor;
out vec4 fragmentColor;
void main(void) {
    gl_Position = vec4(vertexPosition, 0.0, 1.0);
    fragmentColor = vertexColor;
}