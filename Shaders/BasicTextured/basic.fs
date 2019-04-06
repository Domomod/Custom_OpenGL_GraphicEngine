#version 420 core

layout( location = 0 ) out vec4 fragColor;
smooth in vec2 outTexCoord;

layout( binding = 0 ) uniform sampler2D sampler;

void main() {
	fragColor = texture(sampler, outTexCoord);
}
