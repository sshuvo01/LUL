#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;


out VS2FS {
	vec2 _texCoords;
}outBlock;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	outBlock._texCoords = a_TexCoords;
	
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
}
