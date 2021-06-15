#version 330 core

layout(location = 0) in vec3 a_Vertex;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_LightSpaceMatrix * u_Model * vec4(a_Vertex, 1.0);
}