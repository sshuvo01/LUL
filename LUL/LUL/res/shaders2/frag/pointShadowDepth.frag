#version 330 core

in vec4 v_FragPos;

uniform vec3 u_LightPos;
uniform float u_FarPlane;

void main()
{
	float dist = length(u_LightPos - v_FragPos.xyz);

	gl_FragDepth = dist / u_FarPlane;
	//gl_FragDepth = 1.0;
}