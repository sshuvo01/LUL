#shader vertex
#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_VertexNormal;
out vec3 v_FragPosition;

void main()
{
	vec4 temp = u_Model * vec4(vertexNormal, 1.0);
	v_VertexNormal.xyz = temp.xyz;

	temp = u_Model * vec4(vertex, 1.0);
	v_FragPosition.xyz = temp.xyz;

	gl_Position = u_Projection * u_View * u_Model * vec4(vertex, 1.0);
};


#shader fragment
#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 v_VertexNormal;
in vec3 v_FragPosition;

uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform vec3 u_ObjectColor;
uniform vec3 u_CameraPosition;
uniform Material u_Material;
uniform Light u_Light;

out vec4 finalFragColor;

void main()
{
	float ambientStrength = 0.1;
	float specularStrength = 0.3;
	float shininess = 32;

	vec3 ambient = u_Light.ambient * (ambientStrength * u_Material.ambient);
	vec3 diffuse;
	/////////////////
	//v_VertexNormal = normalize(v_VertexNormal);
	vec3 lightDirection = normalize(u_LightPosition - v_FragPosition);

	float diff = max(0.0, dot(v_VertexNormal, lightDirection));
	diffuse = u_Light.diffuse * (diff * u_Material.diffuse);
	
	// specular light
	vec3 cameraDirection = normalize(u_CameraPosition - v_FragPosition);
	vec3 reflectDirection = reflect(-lightDirection, v_VertexNormal);
	float temp = max(0.0, dot(reflectDirection, cameraDirection));
	float foo = specularStrength * pow(temp, u_Material.shininess);
	vec3 specular = u_Light.specular * (foo * u_Material.specular);
	// final result
	vec3 result = (ambient + diffuse + specular);
	finalFragColor = vec4(result, 1.0);
	//finalFragColor = vec4(1.0, 0.0, 1.0, 1.0);
};