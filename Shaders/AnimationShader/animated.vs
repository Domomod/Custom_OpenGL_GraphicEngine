#version 420 core

layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec2 texCoord;
layout( location = 2 ) in vec4 boneIDs;
layout( location = 3 ) in vec4 weights;

smooth out vec2 outTexCoord;

const int MAX_BONES = 50;

layout (std140, binding = 2) uniform block{
mat4 ModelViewProjection;
mat4 bones[MAX_BONES];
};

void main(){
    ivec4 intBoneIDs = floatBitsToInt(boneIDs);

    mat4 boneTransform = bones[intBoneIDs.x] * weights.x
                       + bones[intBoneIDs.y] * weights.y
                       + bones[intBoneIDs.z] * weights.z
                       + bones[intBoneIDs.w] * weights.w;

    outTexCoord = texCoord;
    gl_Position = ModelViewProjection * boneTransform* position;
}