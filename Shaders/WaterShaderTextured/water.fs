#version 420 core

layout( location = 0 ) out vec4 fragColor;

uniform sampler2D sampler;

in vec2 texCord;


void main() {
	fragColor = texture(sampler, texCord);
}
