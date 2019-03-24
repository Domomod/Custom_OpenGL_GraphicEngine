#version 420 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 color;
layout( location = 2 ) in mat4 Model;
smooth out vec4 smoothColor;

out vec3 vPosition;
out vec3 vColor;
out mat4 vM;


void main(){
    vPosition = position;
    vColor    = color;
    vM      = Model;
}