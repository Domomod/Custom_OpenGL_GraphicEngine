#version 420 core

layout( location = 0 ) in vec3 position;
smooth out vec4 smoothColor;
uniform mat4 ModelViewProjection = mat4(1);

void main(){
    smoothColor = vec4(1,1,1, 1);
    gl_Position = ModelViewProjection * vec4(position, 1);
}