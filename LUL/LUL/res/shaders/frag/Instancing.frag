#version 330 core


in VS2FS{
	vec3 _color;
}var;

out vec4 fragColor;

void main()
{
	fragColor = vec4(var._color, 1.0);
}
