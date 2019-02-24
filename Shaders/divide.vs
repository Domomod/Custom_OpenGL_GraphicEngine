#version 420 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 color;

out vec3 controlPosition;
out vec3 controlColor;

void main(){
    controlPosition = position;
    controlColor = color;
}