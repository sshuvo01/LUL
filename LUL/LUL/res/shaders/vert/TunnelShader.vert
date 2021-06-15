#version 330 core

layout(location = 0) in vec3 a_Vertex;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform bool u_ReverseNormal;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_TexCoord;

void main()
{
	vec3 normal	= u_ReverseNormal ? -a_Normal : a_Normal;
	mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
	v_Normal = normalize(normalMatrix * normal);

	v_FragPos = vec3(u_Model * vec4(a_Vertex, 1.0));
	v_TexCoord = a_TexCoord;

	gl_Position = u_Projection * u_View * u_Model * vec4(a_Vertex, 1.0);
}