#version 420 core

layout( location = 0 ) out vec4 fragColor;

in VS_OUT{
    vec3 fragPos_world;
}fs_in;

layout( binding = 8 ) uniform samplerCube skyBox;

vec3 rgbe2rgb(vec4 rgbe) {
  return (rgbe.rgb * pow(2.0, rgbe.a * 255.0 - 128.0));
}

void main() {
    vec3 texCoord = normalize(fs_in.fragPos_world);
    fragColor = texture(skyBox, texCoord);
}
