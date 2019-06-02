#version 440  

in vec3 Position;  //position in camera coordinate
in vec3 Normal;	   //normals
in vec2 TexCoord;  //coordinate for texture 

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float shininess;

struct LightInfo {
	vec4 Position;
	vec4 Direction;
	vec3 Intensity; //color
};

uniform sampler2D texture_diffuse1;

uniform LightInfo Light;

layout (location = 0) out vec4 fragColor;  //r,g,b,a

void main()
{

	vec3 L = normalize(Light.Position.xyz - Position);
	vec3 N = normalize(Normal);
	vec3 V = normalize(-Position);
	vec3 R = normalize(reflect(-L, Normal));
	
	vec3 ambient = Light.Intensity * Ka;	
	vec3 diffuse = Kd * Light.Intensity * max(dot(L,N), 0.0);;
	vec3 specular = Ks * Light.Intensity * pow(max(dot(R,V), 0.0), shininess);

	vec3 color = ambient + diffuse + specular;


	vec4 texColor = texture(texture_diffuse1, TexCoord);

	fragColor = (vec4(color, 1.0) * texColor);
}