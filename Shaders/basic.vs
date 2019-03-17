#version 420 core

layout( location = 0 ) in vec3 position;
smooth out vec4 smoothColor;

layout (std140, binding = 0) uniform testBlock{
vec4 color;
mat4 ModelViewProjection;
};

void main(){
    smoothColor = color;
    gl_Position = ModelViewProjection * vec4(position, 1);
}