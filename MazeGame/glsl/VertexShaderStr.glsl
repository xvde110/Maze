#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform mat4 shadowModelViewMatrix;


out vec2 TexCoord;
out vec3 Normal;
out vec3 Position;

out vec4 shadowCoord;
void main()
{
	gl_Position=projectionMatrix * viewMatrix * vec4(position,1.0);

	mat4 shadowMVPMatrix=projectionMatrix * shadowModelViewMatrix;

	vec4 light0pos = shadowMVPMatrix * vec4(position,1.0);
	shadowCoord = light0pos;

	//gl_Position=light0pos;

	TexCoord=texCoord;
	Normal=normal;
	Position=position;

}