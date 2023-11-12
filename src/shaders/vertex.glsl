layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
out vec4 fragmentColor;
void main(){
    gl_Position = vec4(vertexPosition_modelspace, 1.0);
    fragmentColor = vec4(vertexColor, 1.0);
}