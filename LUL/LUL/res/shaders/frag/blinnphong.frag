#version 330 core

in VS2FS 
{
	vec3 _fragPos;
	vec3 _normal;
	vec2 _texCoords;
}inBlock;

uniform sampler2D u_PlaneTex;
uniform vec3 u_CameraPos;
uniform vec3 u_LightPos;
uniform bool u_Blinn;

out vec4 fragColor;

void main()
{
	vec3 texColor = texture(u_PlaneTex, inBlock._texCoords).rgb;
	vec3 normal = normalize(inBlock._normal);
	vec3 lightDir = normalize(u_LightPos - inBlock._fragPos);
	// ambient component
	vec3 ambient = 0.05 * texColor;
	// diffuse component
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * texColor;
	// specular component
	vec3 viewDir = normalize(u_CameraPos - inBlock._fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	float spec = 0.0;
	if(u_Blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	}
	else
	{
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
	}

	vec3 specular = vec3(0.3) * spec; // assuming bright white light color
	fragColor = vec4(ambient+diffuse+specular, 1.0);
	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}