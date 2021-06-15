#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
//uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	v_TexCoord = texCoord;
	gl_Position = u_Projection * u_View * u_Model * vec4(position, 1);
};

//dasdfasdf
#shader fragment
#version 330 core

out vec4 FragColor;
in vec2 v_TexCoord;
//layout(location = 0) out vec4 color;
//uniform vec4 u_Color;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
	//aglor = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 tex1color = texture(u_Texture1, v_TexCoord);
	vec4 tex2color = texture(u_Texture2, v_TexCoord);

	FragColor = mix(tex1color, tex2color, 0.5);
};