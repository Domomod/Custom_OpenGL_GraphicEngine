#version 420 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 color;
smooth out vec4 smoothColor;
uniform mat4 ModelViewProjection;
uniform float time;
uniform vec3 center;

const float amplitude = 0.05;
const float frequency = 4;
const float PI = 3.14159;

void main(){
    float distance = length(position - center);
    float waveHeight = amplitude * sin(-PI * distance * frequency + time);
    float waveCorrectedY = position.y + waveHeight;

    smoothColor = vec4(color , 1);
    gl_Position = ModelViewProjection * vec4(position.x, waveCorrectedY, position.z, 1);
}