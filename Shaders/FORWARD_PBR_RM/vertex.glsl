#version 420 core

layout( location = 0 ) in vec3 vertPos_model;
layout( location = 1 ) in vec2 vertTexCoords;
layout( location = 2 ) in vec3 vertNormal_model;
layout( location = 3 ) in vec3 vertTangent_model;

out VS_OUT{
    mat3 TBN;
    vec2 fragTexCoord;
    vec3 fragPos_world;
    vec3 lightPos_world;
    vec3 viewerPos_world;
} vs_out;

layout (std140, binding = 0) uniform block{
mat4 MVP;
mat4 M;
vec3 viewerPos_world;
};

void main(){
    /*Temporary data*/
    vec3 lightPos_world = vec3(-10,0,0);
    /* Transform relevant data to world space.*/
    vec3 fragPos_world = vec3(M * vec4(vertPos_model, 1.f));
    /* Calculate to tangent space matrix */
    vec3 N = normalize( vec3(M * vec4(vertNormal_model ,0.f)) );
    vec3 T = normalize( vec3(M * vec4(vertTangent_model,0.f)) );
    //T      = normalize(T - dot(T, N) * N); /* Gram-Schmdt's re-orthogonalization of T with respect to N */
    vec3 B = cross(T, N);

    mat3 TBN = mat3(T,B,N);
    vs_out.TBN             = TBN;
    vs_out.lightPos_world  = lightPos_world;
    vs_out.viewerPos_world = viewerPos_world;
    vs_out.fragTexCoord    = vertTexCoords;
    vs_out.fragPos_world   = fragPos_world;

    /* Calculate vertex screen position */
    gl_Position   = MVP * vec4(vertPos_model, 1.f);
}