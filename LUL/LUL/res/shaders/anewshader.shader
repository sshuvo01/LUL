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

in vec3 v_VertexNormal;
in vec3 v_FragPosition;

uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform vec3 u_ObjectColor;

out vec4 finalFragColor;

void main()
{
	float ambientStrength = 0.1;

	vec3 ambient = u_LightColor * ambientStrength;
	vec3 diffuse;
	/////////////////
	//v_VertexNormal = normalize(v_VertexNormal);
	vec3 lightDirection = normalize(u_LightPosition - v_FragPosition);

	float diff = max(0.0, dot(v_VertexNormal, lightDirection));
	diffuse = u_LightColor * diff;
	
	vec3 result = (ambient + diffuse) * u_ObjectColor;
	finalFragColor = vec4(result, 1.0);
	//finalFragColor = vec4(1.0, 0.0, 1.0, 1.0);
};