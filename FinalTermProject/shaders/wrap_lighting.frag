#version 440

in vec3 Position; 
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float shininess;

struct LightInfo {
	vec4 Position;
	vec3 Intensity;
};

uniform LightInfo Light;
layout (location = 0) uniform  sampler2D  depthMap;

//we need to create at least one "out vec4" for settting fragment colors
layout (location = 0) out vec4 fragColor;  //r,g,b,a

void main()
{
	float wrap = 0.5; 
  float scatterWidth = 0.3;
  vec3 scatterColor = vec3(0.3, 0.0, 0.0);

  float power = 1.5 ;
  float scale = 0.5;

	vec3 L = normalize(Light.Position.xyz - Position);
	vec3 N = normalize(Normal);
	vec3 V = normalize(-Position);
	vec3 R = normalize(reflect(-L, Normal));

	vec3 H = normalize(L + N * wrap);
	float I = pow(clamp(dot(V, -H), 0.0, 1.0), power) * scale;
	
	vec3 ambient = Ka * Light.Intensity;
	vec3 specular = Ks * Light.Intensity * pow(max(dot(L, N), 0.0), shininess);
	vec3 diffuse = Kd * Light.Intensity * max(dot(L, N), 0.0);
	vec3 transluscent;

	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	float dLightToBackSide = texture(depthMap, projCoords.xy).r;
	float dLightToFrontSide = FragPosLightSpace.z;
	
	float absorption = dLightToFrontSide - dLightToBackSide;

	//vec3 E = normalize( - vViewPosition .xyz );	
	
	//float contribution = 0.75 - 0.25 * dot(-uLightViewDirection.xyz, E);
	
	vec3 translucent = I * pow( exp(-absorption ), 4.0 ) * (Light.Intensity + ambient);

	// Output final color.
	//fragColor.rgb = ambient + attenuation * ( shadow * ( specular + diffuse )  + rim + translucent );

	//vec3 finalColor = ambient + specular + diffuse + Kd * I;

	vec3 finalColor = ambient * specular + diffuse + translucent;
	
	fragColor = vec4(finalColor, 1.0);
}