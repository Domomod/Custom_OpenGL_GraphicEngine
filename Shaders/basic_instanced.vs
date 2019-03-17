#version 420 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in mat4 Model;
smooth out vec4 smoothColor;

layout (std140, binding = 0) uniform data{
vec4 color;
mat4 View;
mat4 Projection;
};

void main(){
    smoothColor = color;
    gl_Position = Projection * View * Model * vec4(position, 1);
}