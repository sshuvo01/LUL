#version 330 core

in vec3 v_TexDir;

uniform samplerCube u_SkyboxTexture;

out vec4 fragColor;

void main()
{
	fragColor = texture(u_SkyboxTexture, v_TexDir);
	//fragColor = vec4(0.5, 0.2, 0.23, 1.0);
}