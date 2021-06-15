#version 330 core
layout(location = 0) in vec3 a_Vertex;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4	u_Model;
uniform mat4	u_View;
uniform mat4	u_Projection;
uniform bool	u_ReverseNormal;

out VS2FS
{
	vec3 normal;
	vec3 fragPos;
	vec2 texCoord;
} outBlock;

void main()
{
	if(u_ReverseNormal)
	{
		outBlock.normal = transpose(inverse(mat3(u_Model))) 
			* (-1.0 * a_Normal);
	}
	else
	{
		outBlock.normal = transpose(inverse(mat3(u_Model))) 
			* a_Normal;
	}
	outBlock.fragPos = vec3(u_Model * vec4(a_Vertex,1.0));
	outBlock.texCoord = a_TexCoord;
	gl_Position = u_Projection*u_View*u_Model * vec4(a_Vertex, 1.0);
}