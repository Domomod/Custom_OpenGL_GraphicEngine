#version 420 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 color;
layout( location = 2) in mat4 Model;
smooth out vec4 smoothColor;

layout (std140, binding = 0) uniform data{
mat4 View;
mat4 Projection;
};

void main(){
    smoothColor = vec4(color,1);
    gl_Position = Projection * View * Model * vec4(position, 1);
}