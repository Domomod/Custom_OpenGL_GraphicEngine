#version 420 core

layout( location = 0 ) out vec4 fragColor;

in vec4 color;


void main() {
	fragColor = color;
}
