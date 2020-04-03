#version 330 core

uniform sampler2D u_BoxTexture;
out vec4 finalFragColor;
in vec2 v_TexCoord;

void main()
{
	finalFragColor = texture(u_BoxTexture, v_TexCoord);
	//vec4(1.0, 1.0, 1.0, 1.0);// *vec4(0.45, 0.6, 0.7, 1.0);
}