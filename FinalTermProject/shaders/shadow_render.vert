#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 fragPos;
out vec3 fragNormal;
out vec4 fragPosLightSpace;

uniform mat4 mvp;
uniform mat4 modelView;
uniform mat3 NormalMatrix;
uniform mat4 shadowMVP;

uniform mat4 lightMVP;

void main()
{

	fragNormal = normalize(NormalMatrix * normal);
	fragPos = vec3(modelView * vec4(position,1.0));
	TexCoords = texCoords;
	fragPosLightSpace = shadowMVP * vec4(position,1.0);
	gl_Position = mvp * vec4(position, 1.0f);
}