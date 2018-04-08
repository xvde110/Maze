#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 Position;

in vec4 shadowCoord;

out vec4 color;

uniform vec3 ambient;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform sampler2D shadowTexture;
uniform sampler2D texture1;

void main()
{  
	vec4 texColor=texture(texture1,TexCoord);
	vec3 lightDir=lightPos-vec3(Position);
	lightDir=normalize(lightDir);


	float diffuse=max(0.0,dot(Normal,lightDir));
	
	vec3 projCoords=shadowCoord.xyz/shadowCoord.w;	
	projCoords = projCoords * 0.5 + 0.5;
	float closestDis = texture(shadowTexture,projCoords.xy).r;
	float dis=projCoords.z;
	float offsetDepth=0.00005;
	float depthFactor = dis - offsetDepth > closestDis ? 0.0 : 1.0;

	color=vec4((ambient + diffuse * lightColor * depthFactor),1.0)*texColor;
	color.a=1.0;

	//color=vec4(vec3(closestDis),1.0);

	//if(abs(closestDis-1.0)<0.000001) color=vec4(1.0,1.0,1.0,1.0);
}