#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

out vec4 fragColor;

void main()
{
	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);

	fragColor = texture(u_Texture, v_TexCoord);
}