#version 330 core
layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = texCoord;
	gl_Position = vec4(vertex.x, vertex.y, 0.0, 1.0);	
}