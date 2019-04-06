#version 420 core

layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec2 texCoord;
layout( location = 2) in mat4 Model;
smooth out vec2 outTexCoord;

layout (std140, binding = 0) uniform data{
mat4 View;
mat4 Projection;
};

void main(){
    outTexCoord = texCoord;
    gl_Position = (Projection * View * Model) * position;
}