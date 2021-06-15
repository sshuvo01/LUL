#version 330 core

in VS2FS 
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoord;
	vec4 fragPosLightSpace;
} inputBlock;

uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_DepthMap;

out vec4 fragColor;

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0, 1] range
	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(u_DepthMap, projCoords.xy).r;

	float currentDepth = projCoords.z;
	
	if(currentDepth > 1.0)
	{
		return 0.0;
	}
	// PCF
	float shadow = 0.0;
	vec2 texelSIze = 1.0/textureSize(u_DepthMap, 0);
	for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			vec2 coord = projCoords.xy + vec2(x, y) * texelSIze;
			float pcfDepth = texture(u_DepthMap, coord).r;
			shadow += (currentDepth-bias > pcfDepth ? 1.0 : 0.0);
		}
	}

	shadow = shadow / 9.0;

	return shadow;
	//return closestDepth > currentDepth-bias ? 0.0 : 1.0;
}

void main()
{
	vec3 color = texture(u_DiffuseTexture, inputBlock.texCoord).rgb;
	vec3 normal = normalize(inputBlock.normal);
	vec3 lightColor = vec3(0.3);
	// ambient
	vec3 ambient = 0.3 * color;
	// diffuse
	vec3 lightDir = normalize(u_LightPos - inputBlock.fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(u_CameraPos - inputBlock.fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;
	// calculate shadow 
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float shadow = ShadowCalculation(inputBlock.fragPosLightSpace, bias);

	vec3 lighting = (ambient + (1.0 - shadow) * 
		(diffuse + specular)) * color;
	fragColor = vec4(lighting, 1.0);
}