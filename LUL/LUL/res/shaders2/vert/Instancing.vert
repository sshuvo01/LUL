#version 330 core
layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_Offset;

out VS2FS{
	vec3 _color;
}var;

//uniform vec2 u_Offsets[100];

void main()
{
	vec2 pos = a_Pos * (gl_InstanceID/100.0);
	var._color = a_Color;
	gl_Position = vec4(pos + a_Offset, 0.0, 1.0);
}