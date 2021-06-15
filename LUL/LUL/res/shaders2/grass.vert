#shader vertex
#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 v_texCoord;


void main()
{
	v_texCoord = texCoord;
	gl_Position = u_Projection * u_View * u_Model * vec4(vertex, 1);
}

#shader fragment
#version 330 core

in vec2 v_texCoord;

uniform sampler2D u_GrassTexture;

out vec4 fragColor;

void main()
{
	vec4 texValue;
	texValue = texture(u_GrassTexture, v_texCoord);

	if(texValue.w < 0.1)
	{
		discard;
	}

	fragColor = texValue;
}