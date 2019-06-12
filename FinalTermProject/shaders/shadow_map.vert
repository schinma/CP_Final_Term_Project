#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 lightMVP;

void main()
{
	gl_Position = lightMVP * vec4(position, 1.0f);
}