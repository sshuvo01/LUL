#version 330 core

struct Light
{
	vec3 position;
	vec3 color;
};

uniform Light u_Lights[16];
uniform int u_LightN;
uniform sampler2D u_DiffuseTexture;
uniform vec3 u_ViewPos;

in vec3 v_Normal;
in vec3 v_FragPos;
in vec2 v_TexCoord;

layout(location = 0) out vec4 finalColor;
layout(location = 1) out vec4 newColor;

void main()
{
	vec3 color = texture(u_DiffuseTexture, v_TexCoord).rgb;

	vec3 ambient = 0.0 * color;
	vec3 lighting = vec3(0.0);

	for(int i = 0; i < u_LightN; i++)
	{
		vec3 lightDir = normalize(u_Lights[i].position - v_FragPos);
		float diff = max(dot(lightDir, v_Normal), 0.0);
		vec3 diffuse = u_Lights[i].color * diff * color;
		vec3 result = diffuse;

		float dist = length(v_FragPos - u_Lights[i].position);
		result *= 1.0 / (dist * dist);
		lighting += result;
	}
	finalColor = vec4(ambient + lighting, 1.0);
	float brightness = dot(finalColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	
	if(brightness > 1.0) newColor = finalColor;
	else newColor = vec4(0.0, 0.0, 0.0, 1.0);
}