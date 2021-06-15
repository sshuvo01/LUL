#version 330 core

uniform sampler2D u_BoxTexture;
uniform samplerCube u_SkyboxTexture;
uniform vec3 u_CameraPos;

out vec4 finalFragColor;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

void main()
{
	vec3 I = normalize(v_FragPos - u_CameraPos);
	vec3 R = reflect(I, normalize(v_Normal));
	vec4 boxTexel = texture(u_BoxTexture, v_TexCoord);
	vec4 reflection = vec4(texture(u_SkyboxTexture, R).rgb, 1.0);
	
	/*calculate refraction*/
	float ratio = 1.00/1.52; 
	R = refract(I, normalize(v_Normal), ratio);
	vec4 refraction = vec4(texture(u_SkyboxTexture, R).rgb, 1.0);

	finalFragColor = refraction * 0.4 + reflection * 0.6;

	//vec4(1.0, 1.0, 1.0, 1.0);// *vec4(0.45, 0.6, 0.7, 1.0);
}