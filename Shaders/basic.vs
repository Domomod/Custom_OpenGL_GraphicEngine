#version 420 core

layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec4 color;
smooth out vec4 smoothColor;

layout (std140, binding = 0) uniform block{
mat4 ModelViewProjection;
};

void main(){
    smoothColor = color;
    gl_Position = ModelViewProjection * position;
}