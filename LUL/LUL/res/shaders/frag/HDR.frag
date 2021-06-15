#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

out vec4 finalColor;

// each fragment
void main()
{
	float gamma = 2.2;
	vec3 texColor = texture(u_Texture, v_TexCoord).rgb;
	vec3 gammaColor = pow(texColor, vec3(1.0/gamma));
	//finalColor = vec4(gammaColor, 1.0);

	vec3 mappedColor = texColor / (texColor + vec3(1.0));
	gammaColor = pow(mappedColor, vec3(1.0/gamma));
	finalColor = vec4(gammaColor, 1.0);
//	const float gamma = 2.2;
//	vec3 hdrColor = texture(u_Texture, v_TexCoord).rgb;
//
//	// reinhard tone mapping
//	vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
//	// gamma correction 
//	mapped = pow(mapped, vec3(1.0 / gamma));
//	finalColor = vec4(mapped, 1.0);
}