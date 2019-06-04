#version 420 core

layout( location = 0 ) out vec4 fragColor;

in VS_OUT{
    vec3 fragPos_world;
}fs_in;

layout( binding = 0 ) uniform samplerCube skyBox;

void main() {
    vec3 texCoord = normalize(fs_in.fragPos_world);
    fragColor = texture(skyBox, texCoord);
}
