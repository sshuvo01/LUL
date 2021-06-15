#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

out VS2FS
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} outBlock;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main()
{
	outBlock.FragPos = vec3(u_Model * vec4(a_Position, 1.0));
	outBlock.TexCoord = a_TexCoord;

	mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
	vec3 T = normalize(normalMatrix * a_Tangent);
	vec3 N = normalize(normalMatrix * a_Normal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(B, T, N));
	outBlock.TangentLightPos = TBN * u_LightPos;
	outBlock.TangentViewPos = TBN * u_ViewPos;
	outBlock.TangentFragPos = TBN * outBlock.FragPos;
	
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}