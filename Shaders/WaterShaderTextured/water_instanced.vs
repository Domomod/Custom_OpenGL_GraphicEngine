#version 420 core

layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec2 texCord;
layout( location = 2 ) in mat4 Model;
smooth out vec4 smoothColor;

out vec4 vPosition;
out vec2 vTexCord;
out mat4 vM;


void main(){
    vPosition = position;
    vTexCord  = texCord;
    vM        = Model;
}