#shader vertex
#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_VertexNormal;
out vec3 v_FragPosition;
out vec2 v_TexCoord;

void main()
{
	//vec4 temp = u_Model * vec4(vertexNormal, 1.0);
	v_VertexNormal = mat3(transpose(inverse(u_Model))) * vertexNormal;
	//v_VertexNormal.xyz = temp.xyz;

	vec4 temp = u_Model * vec4(vertex, 1.0);
	v_FragPosition.xyz = temp.xyz;

	v_TexCoord = texCoord;
	gl_Position = u_Projection * u_View * u_Model * vec4(vertex, 1.0);
};


#shader fragment
#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 v_VertexNormal;
in vec3 v_FragPosition;
in vec2 v_TexCoord;

uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform vec3 u_ObjectColor;
uniform vec3 u_CameraPosition;
uniform Material u_Material;
uniform Light u_Light;

out vec4 finalFragColor;

//
void ComputeAmbient(out vec3 ambient)
{
	ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, v_TexCoord));
}
//
void ComputeDiffuse(out vec3 diffuse)
{
	vec3 norm = normalize(v_VertexNormal);
	vec3 lightDirection = normalize(u_LightPosition - v_FragPosition);

	float diff = max(0.0, dot(norm, lightDirection));
	vec3 texel = vec3(texture(u_Material.diffuse, v_TexCoord));
	diffuse = u_Light.diffuse * (diff * texel);
}
//
void ComputeSpecular(out vec3 specular)
{
	vec3 norm = normalize(v_VertexNormal);

	float specularStrength = 0.3;
	vec3 lightDirection = normalize(u_LightPosition - v_FragPosition);

	vec3 cameraDirection = normalize(u_CameraPosition - v_FragPosition);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float temp = max(0.0, dot(reflectDirection, cameraDirection));
	float foo =  pow(temp, u_Material.shininess);
	vec3 texel = vec3(texture(u_Material.specular, v_TexCoord));
	specular = u_Light.specular * (foo * texel);
}
//
float GetAttenuation(in float distance)
{
	float attenuation = 1.0 / (u_Light.constant + u_Light.linear * distance +
		u_Light.quadratic * (distance * distance));

	return attenuation;
}
//
void main()
{
	float ambientStrength = 0.1;
	float shininess = 32;

	vec3 ambient;
	ComputeAmbient(ambient);

	vec3 diffuse;
	ComputeDiffuse(diffuse);
	
	vec3 specular;
	ComputeSpecular(specular);
	//specular = vec3(texture(u_Material.specular, v_TexCoord));
	
	float distance = length(v_FragPosition - u_LightPosition);
	float attenuation = GetAttenuation(distance);
	// final result
	//ambient *= attenuation;
	//diffuse *= attenuation;
	//specular *= attenuation;

	vec3 result = (ambient + diffuse + specular);
	finalFragColor = vec4(result, 1.0);
	//finalFragColor = vec4(1.0, 0.0, 1.0, 1.0);
};