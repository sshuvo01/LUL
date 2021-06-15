#version 330 core

layout(location = 0) in vec2 coord;
layout(location = 1) in vec3 color;

out VS2GS{
	vec3 _color;
}vsOut;

void main()
{
	vsOut._color = color;
	gl_Position = vec4(coord.x, coord.y, 0.0, 1.0);	
}