#version 420 core

layout( location = 0 ) out vec4 fragColor;

in VS_OUT{
    vec3 fragPos_world;
}fs_in;

layout( binding = 0 ) uniform sampler2D equirectangularMap;

/*Reverse of 2PI and PI*/
const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    /*Polar coordinates*/
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    /*Reverse projection from equirectangular map*/
    uv *= invAtan;
    /*Transform from <-.5 .5> to <0,1>*/
    uv += 0.5;
    return uv;
}

void main() {
    /*Since the cube is in 0,0,0 position it's
    normalized fragment positions would be normals of
    a unit cube that is also placed in the 0,0,0*/
    vec3 fragNormal_world = normalize(fs_in.fragPos_world);
    vec2 uv = SampleSphericalMap(normalize(fragNormal_world));
    vec3 color = texture(equirectangularMap, uv).rgb;

    fragColor = vec4(color, 1.0);
}
