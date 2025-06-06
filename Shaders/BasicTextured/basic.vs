#version 420 core

layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec2 texCoord;

smooth out vec2 outTexCoord;

layout (std140, binding = 0) uniform block{
mat4 ModelViewProjection;
};

void main(){
    outTexCoord = texCoord;
    gl_Position = ModelViewProjection * position;
}