#version 420 core

layout( location = 0 ) in vec4 position;
smooth out vec4 smoothColor;

layout (std140, binding = 0) uniform block{
mat4 ModelViewProjection;
};

void main(){
    gl_Position = ModelViewProjection * position;
    smoothColor = vec4(1.0);
}