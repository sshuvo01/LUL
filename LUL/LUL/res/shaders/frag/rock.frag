#version 330 core

in VS2FS {
	vec2 _texCoords;
}inBlock;

uniform sampler2D texture_diffuse1;

out vec4 fragColor;

void main()
{
	fragColor = texture(texture_diffuse1, inBlock._texCoords);
}