#version 440

in vec3 Position; 
in vec3 Normal;

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float shininess;

struct LightInfo {
	vec4 Position;
	vec3 Intensity;
};

uniform LightInfo Light;

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

	vec3 finalColor = ambient + specular + diffuse + Kd * I;

	//float NdotL = dot(N, L);
	//float NdotL_wrap = (NdotL + wrap) / (1 + wrap);
	//float wrap_diffuse = max(0, NdotL_wrap);

	//float scatter = smoothstep(0.0, scatterWidth, NdotL_wrap);

	//vec3 specular =   Ks * Light.Intensity * (NdotL_wrap <= 0 ? 0 : pow(max(dot(R,V), 0.0), shininess));
	//vec3 diffuse = Kd * Light.Intensity * wrap_diffuse;

	//vec3 finalColor = ambient + diffuse + scatter * scatterColor + specular;

	fragColor = vec4(finalColor, 1.0);
}