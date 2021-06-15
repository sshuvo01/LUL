#version 330 core

out vec4 fragColor;

in GS2FS{
	vec3 _color;
}fsIn;

void main()
{
	fragColor = vec4(fsIn._color, 1.0);
}