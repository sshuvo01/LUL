#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS2GS{
	vec3 _color;
}gsIn[];

out GS2FS
{
	vec3 _color;
}gsOut;

void build_house(vec4 position)
{
	gsOut._color = gsIn[0]._color;
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); 
	EmitVertex();
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);
	EmitVertex();
	gsOut._color = vec3(1.0, 1.0, 1.0);
	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	build_house(gl_in[0].gl_Position);
}