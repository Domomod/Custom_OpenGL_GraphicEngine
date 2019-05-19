#version 420 core

layout( location = 0 ) in vec3 vertPos_model;

layout (std140, binding = 0) uniform block{
        mat4 P;
        mat4 V;
};

out VS_OUT{
    vec3 fragPos_world;
}vs_out;

void main(){
    vs_out.fragPos_world = vertPos_model;

    /*Skybox moves with the camera so I remove the translation part of the matrix.*/
    mat4 V_noTranslation = mat4(mat3(V));

    /*Sky box is a 1x1x1 cube and it's model pos are also in world pos.*/
    vec4 pos = P * V_noTranslation * vec4(vertPos_model, 1.0);

    /* glPosition's z value quals w. This way when performing the perspective division
     it's z value will equal to 1, a maximum possible depth value. Therefore any object will
     obfuscate the skybox, which means GPU calls less fragment shader runs to for the skybox.*/
    gl_Position = pos.xyww;
}