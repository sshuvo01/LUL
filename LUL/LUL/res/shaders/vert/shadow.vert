#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out VS2FS 
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoord;
	vec4 fragPosLightSpace;
} outputBlock;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_LightSpaceMatrix;

void main()
{
	outputBlock.fragPos = vec3(u_Model * vec4(a_Position, 1.0));
	outputBlock.normal = transpose(inverse(mat3(u_Model)))*a_Normal;
	outputBlock.texCoord = a_TexCoord;
	outputBlock.fragPosLightSpace = u_LightSpaceMatrix * u_Model 
		* vec4(a_Position, 1.0);
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}