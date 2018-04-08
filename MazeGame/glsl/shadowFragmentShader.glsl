#version 330 core
out vec4 color;
in float depth;
void main()
{  
	color=vec4(depth,depth/2000,depth/65535,1.0);
}