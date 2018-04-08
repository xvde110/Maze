#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightPos;

out float depth;

void main()
{
	gl_Position=projectionMatrix * viewMatrix * vec4(position,1.0);
	depth=distance(lightPos,position);
}