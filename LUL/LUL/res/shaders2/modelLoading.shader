#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_FragPosition;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	v_TexCoords = aTexCoords;
	
	vec4 vVrtx = u_Model * vec4(aPos, 1.0);
	v_FragPosition.xyz = vVrtx.xyz;

	v_Normal = mat3(transpose(inverse(u_Model))) * aNormal;
	
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}


#shader fragment
#version 330 core

in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_FragPosition;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 u_CameraPosition;

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;

	float constant;
	float linear;
	float quadratic;
};

uniform Light u_Light;

//
void ComputeDiffuse(out vec3 diffuse, in vec3 texel)
{

	vec3 norm = normalize(v_Normal);
	vec3 lightDirection = normalize(u_Light.position - v_FragPosition);

	float diff = max(0.0, dot(norm, lightDirection));
	//vec3 texel = vec3(texture(u_Material.diffuse, v_TexCoord));
	diffuse = u_Light.diffuse * (diff * texel);
}
//
void ComputeSpecular(out vec3 specular, in vec3 texel)
{
	vec3 norm = normalize(v_Normal);

	float specularStrength = 1.0;
	vec3 lightDirection = normalize(u_Light.position - v_FragPosition);

	vec3 cameraDirection = normalize(u_CameraPosition - v_FragPosition);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float temp = max(0.0, dot(reflectDirection, cameraDirection));
	float foo = pow(temp, 32);
	//vec3 texel = vec3(texture(u_Material.specular, v_TexCoord));
	specular = u_Light.specular * (foo * texel);
}
//
void main()
{
	vec4 temp = texture(texture_diffuse1, v_TexCoords);
	vec3 diffTexel = temp.xyz;

	temp = texture(texture_specular1, v_TexCoords);
	vec3 specTexel = temp.xyz;

	// ambient light
	vec3 ambient = u_Light.ambient * diffTexel;
	// diffuse light
	vec3 diffuse;
	ComputeDiffuse(diffuse, diffTexel);
	// specular light
	vec3 specular;
	ComputeSpecular(specular, specTexel);

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}