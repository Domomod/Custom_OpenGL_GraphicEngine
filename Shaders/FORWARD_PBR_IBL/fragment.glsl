#version 420 core

layout( location = 0 ) out vec4 fragColor;
smooth in vec2 outTexCoord;

in VS_OUT{
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
} fs_in;

layout (std140, binding = 0) uniform block{
mat4 MVP;
mat4 M;
vec3 viewerPos_world;
float MAX_REFLECTION_LOD;
};

layout( binding = 0 ) uniform sampler2D albedoMap;
layout( binding = 1 ) uniform sampler2D ambientMap;
layout( binding = 2 ) uniform sampler2D metalnessMap;
layout( binding = 3 ) uniform sampler2D roughnessMap;
layout( binding = 4 ) uniform sampler2D normalMap;

layout( binding = 5 ) uniform sampler2D brdfLUT;
layout( binding = 6 ) uniform samplerCube prefilteredEnviromentMap;
layout( binding = 7 ) uniform samplerCube irradianceMap;

const float PI = 3.14159265359;

/* This is so called Fresnel-Schick aproximation of Fresnel equation*/
vec3 calculateSurfaceReflection_FresnelShick(float angleCos, vec3 surfaceReflection){
    return surfaceReflection + (1.0 - surfaceReflection) * pow(1.0 - angleCos, 5.0);
}

vec3 calculateSurfaceReflection_FresnelShick_Roughness(float cosTheta, vec3 surfaceReflection, float roughness)
{
    return surfaceReflection + (max(vec3(1.0 - roughness), surfaceReflection) - surfaceReflection) * pow(1.0 - cosTheta, 5.0);
}

/* This is so called Trowbridge-Reitz GGX function.
Normal Vector Distribution statistically aproximates microfacets alignance to halfway vector.
 Which corelates to how mirror-like reflections are located on surface.*/
float calculateNDF_GGX(vec3 normal, vec3 halfway, float roughness){
    /* Multpilication is a basic operation while pow(x,2) is not
     possibly this aproach might be faster but it's realy
     hardware/compiler dependent. */
    float a = roughness;
    float a2 = a * a;
    float NdotH2 = dot(normal, halfway);
    NdotH2 *= NdotH2;
    float numerator = a2;
    float denominator = NdotH2 * (a2 - 1.0) + 1;
    denominator = PI * denominator * denominator;
    return numerator / denominator;
}

/* Using a combination of the GGX and Schlick-Beckmann approximation known as Schlick-GGX
we can aproximate microfacets overshadowing based on material roughness.*/
float aproximateSurfaceOcclusion_SchickGGX(float NdotV, float roughness){
    /*Accordign to disney and Epic Games this
    small tweak enhances quality of normal distributions
    and visual effects*/
    float r = roughness + 1;
    float k = (r*r) / 8.0;
    return NdotV / (NdotV * (1-k) + k);
}

/*Geometry may overshadow both from viewer perspective (geometry obstruction aka occluding
 microfacet from viewer perspective) and light perspective (geometry shadowing aka casting
 shadow on viewed microfacet). Smiths method takes both into account by simply calculationg
 surface overshadowing for both cases.*/
float calculateCompleteGeometryOcclusion_Smith(float NdotV, float NdotL, float roughness){
    float GeometryObstruction = aproximateSurfaceOcclusion_SchickGGX(NdotV, roughness);
    float GeometryOvershadowing = aproximateSurfaceOcclusion_SchickGGX(NdotL, roughness);
    return GeometryObstruction * GeometryOvershadowing;
}

void main() {

    /*Temporary data*/
    vec3 lightCol = vec3 (1.0, 1.0, 1.0);
    /**/
    vec3 albedo     = texture(albedoMap   , fs_in.fragTexCoord).rgb;
    vec3 ao         = texture(ambientMap  , fs_in.fragTexCoord).rrr;
    float metalness = texture(metalnessMap, fs_in.fragTexCoord).r;
    float roughness = texture(roughnessMap, fs_in.fragTexCoord).r;

    vec3 F0 = vec3(0.04);
    F0      = mix(F0, albedo, metalness);

    vec3 N_tangent = normalize(texture(normalMap, fs_in.fragTexCoord).rgb * 2.0 - 1.0);
    vec3 V_tangent = normalize(fs_in.viewerPos_tangent - fs_in.fragPos_tangent);

    float NdotV = max(dot(N_tangent, V_tangent), 0.0);

    /*Direct ligtning*/
    vec3 outcomingRadiance = vec3(0.0);
    {
        vec3 L_tangent = normalize(fs_in.lightPos_tangent - fs_in.fragPos_tangent);
        vec3 H_tangent = normalize(V_tangent + L_tangent);

        float dist     = length(fs_in.lightPos_tangent - fs_in.fragPos_tangent);
        float att      = 1.0 / (0.05 * dist * dist);
        vec3  radiance = lightCol * att;

        /*F0 is a common math symbol for surface reflectivity at zero incidence.
         For most dielectrics value 0.04 looks good enough. While for metallic surfaces
         I calculate this parameter. For dielectrics albedo stores material color but
         or metal surfaces it stores surface reflecrivity. Techincally surface is either
         metal or dielectric but I support non binary values (Hey let's give the artist
         freedom of design).*/

        float cosTheta = max(dot(H_tangent, V_tangent), 0.0);

        /*Cook-Torrance Bidirectional Reflectance Distribution Function*/
        float NdotL = max(dot(N_tangent, L_tangent), 0.0);
        vec3 F      = calculateSurfaceReflection_FresnelShick(cosTheta, F0);
        float NDF   = calculateNDF_GGX(N_tangent, H_tangent, roughness);
        float G     = calculateCompleteGeometryOcclusion_Smith(NdotL, NdotV, roughness);

        vec3 specContribution = F;
        vec3 diffContrubition = vec3(1.f) - specContribution;

        /*Metalic surfaces do not refract light which means no diffuse reflection*/
        diffContrubition *= 1.0 - metalness;

        /*Fresnel equation (F component) already include specContribution (because its
        relation of light reflected to incident light)*/
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * NdotV * NdotL + 0.0001;
        vec3 specular     = numerator / max(denominator, 0.001); /*0.001 avoids dividing by 0*/
        outcomingRadiance = (diffContrubition * albedo / PI + specular) * radiance * NdotL;
    }

    /*Image Based Lightning - ambient lightning*/
    vec3 F = calculateSurfaceReflection_FresnelShick_Roughness(NdotV, F0, roughness);
    vec3 specContribution = F;
    vec3 diffContrubition = vec3(1.f) - specContribution;

    /*Metalic surfaces do not refract light which means no diffuse reflection*/
    diffContrubition *= 1.0 - metalness;

    vec3 N = normalize(fs_in.TBN * N_tangent);
    vec3 V = normalize(fs_in.TBN * V_tangent);
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;

    vec3 R = normalize( reflect(-V, N) );

    vec3 prefilteredColor = textureLod(prefilteredEnviromentMap, R, roughness * 5 ).rgb;
    vec2 brdf  = texture(brdfLUT, vec2(NdotV, roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (diffContrubition * diffuse + specular) * ao;

    vec3 color =  ambient + outcomingRadiance;
    color = pow(color, vec3(1.0/2.2));   /*Gamma correction*/

	fragColor = vec4(color, 1.0);
}
