#shader vertex
#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(vertex, 1);
};


#shader fragment
#version 330 core

out vec4 finalFragColor;

void main()
{
	finalFragColor = vec4(1.0, 1.0, 1.0, 1.0);// *vec4(0.45, 0.6, 0.7, 1.0);
}