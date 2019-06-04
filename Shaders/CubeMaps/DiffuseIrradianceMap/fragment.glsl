#version 420 core

layout( location = 0 ) out vec4 fragColor;

in VS_OUT{
    vec3 fragPos_world;
}fs_in;

layout( binding = 0 ) uniform samplerCube enviromentMap;

const float PI = 3.14159265359;

void main() {
    vec3 fragNormal_world = normalize(fs_in.fragPos_world);

    vec3 irradiance = vec3(0.0);

    vec3 fragUp_world_   = vec3(0.0, 1.0, 0.0);
    vec3 fragRight_world = cross(fragUp_world_, fragNormal_world);
         fragUp_world_ = cross(fragNormal_world, fragRight_world);

    float delta = 0.025;
    float i = 0.0;
    for(float azimuthAngle = 0.0; azimuthAngle < 2.0 * PI; azimuthAngle += delta){
        for(float zenithAngle =0.0; zenithAngle < 0.5 * PI; zenithAngle += delta){
            vec3 sampleUv_tangent = vec3( sin(zenithAngle) * cos(azimuthAngle),
                                    sin(zenithAngle) * sin(azimuthAngle),
                                    cos(zenithAngle));

            vec3 sampleUv_world = sampleUv_tangent.x * fragRight_world
                                + sampleUv_tangent.y * fragUp_world_
                                + sampleUv_tangent.z * fragNormal_world;

            vec3 component = texture(enviromentMap, sampleUv_world).rgb;
            /*Enviroment light is weaker at larger zenith angles*/
            component *= cos(zenithAngle);
            /*On higher hemishperes samples are closer to each other,
            so i scale their weight down*/
            component *= sin(zenithAngle);

            irradiance += component;
            i++;
        }
    }
    irradiance = PI * irradiance / i;

    fragColor = vec4(irradiance, 1);
}
