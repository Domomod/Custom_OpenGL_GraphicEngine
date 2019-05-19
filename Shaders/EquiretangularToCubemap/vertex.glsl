#version 420 core

layout( location = 0 ) in vec3 vertPos_model;

layout (std140, binding = 0) uniform block{
        mat4 P;
        mat4 V;
};

out VS_OUT{
    vec3 fragPos_world;
}vs_out;

void main()
{
    vs_out.fragPos_world = vertPos_model;
    gl_Position =   P * V * vec4(vertPos_model,1);
}