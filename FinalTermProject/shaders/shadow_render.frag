#version 430

out vec4 color;

in vec2 TexCoords;
in vec3 fragPos;
in vec3 fragNormal;
in vec4 fragPosLightSpace;

layout (location = 0) uniform sampler2D depth;
layout (location = 1) uniform sampler2D texture_diffuse1;
uniform int hasTextureDiffuse;

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float shininess;

struct LightInfo {
	vec4 Position;
	vec3 Intensity;
};

uniform LightInfo light;

float ShadowCalculation(vec4 PInLightSpace)
{
    // perform perspective divide
    vec3 projCoords = PInLightSpace.xyz / PInLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;  //why? because it has a values [-1, 1], convert to [0, 1]
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depth, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
    // Supress the limit of the far / close plane
    if (projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}


void main()
{
	vec3 L = normalize(light.Position.xyz - fragPos);
	vec3 N = normalize(fragNormal);
	vec3 V = normalize(-fragPos);
	vec3 R = normalize(reflect(-L, fragNormal));

	vec3 ambient = light.Intensity * Ka;
	vec3 specular = Kd * light.Intensity * max(dot(L,N), 0.0);;
	vec3 diffuse = Ks * light.Intensity * pow(max(dot(R,V), 0.0), shininess);

	vec3 c = vec3(1.0);

	if (hasTextureDiffuse == 1) {
		c = texture(texture_diffuse1, TexCoords).rgb;
	}

	float shadow = ShadowCalculation(fragPosLightSpace);
	vec3 final = (ambient + (1-shadow)*(diffuse + specular))*c;
	color = vec4(final, 1.0);
}