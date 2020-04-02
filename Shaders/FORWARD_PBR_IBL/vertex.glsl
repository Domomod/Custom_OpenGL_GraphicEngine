#version 420 core

layout( location = 0 ) in vec3 vertPos_model;
layout( location = 1 ) in vec3 vertNormal_model;
layout( location = 2 ) in vec3 vertTangent_model;

layout( location = 4 ) in vec2 vertTexCoords;

out VS_OUT{
    vec2 fragTexCoord;
    vec3 fragPos_world;
    vec3 fragPos_tangent;
    vec3 lightPos_tangent;
    vec3 viewerPos_tangent;
    mat3 TBN;
    /*bellow for debugging*/
    vec3 T;
    vec3 B;
    vec3 N;
} vs_out;

layout (std140, binding = 0) uniform block{
mat4 MVP;
mat4 M;
vec3 viewerPos_world;
float MAX_REFLECTION_LOD;
};

void main(){
    /*Temporary data*/
    vec3 lightPos_world = vec3(0.3,5.0,0.6);
    /* Transform relevant data to world space.*/
    vec3 fragPos_world = vec3(M * vec4(vertPos_model, 1.f));
    /* Calculate to tangent space matrix */
    vec3 N = normalize( vec3(M * vec4(vertNormal_model ,0.f)) );
    vec3 T = normalize( vec3(M * vec4(vertTangent_model,0.f)) );
    T      = normalize(T - dot(T, N) * N); /* Gram-Schmdt's re-orthogonalization of T with respect to N */
    vec3 B = normalize( cross(T, N) );
    /* By transforming light calculation relevant vectors to tangent space
     we avoid matrix multiplication in fragment shader as they are in the same
     space as normal map's normals */
    mat3 TBN = transpose(mat3(T,B,N));
    vs_out.fragPos_tangent   = TBN * fragPos_world;
    vs_out.lightPos_tangent  = TBN * lightPos_world;
    vs_out.viewerPos_tangent = TBN * viewerPos_world;
    vs_out.fragTexCoord  = vertTexCoords;
    vs_out.fragPos_world = fragPos_world;
    vs_out.TBN = mat3(T,B,N);
    vs_out.T = T;
    vs_out.B = B;
    vs_out.N = N;
    /* Calculate vertex screen position */
    gl_Position   = MVP * vec4(vertPos_model, 1.f);
}