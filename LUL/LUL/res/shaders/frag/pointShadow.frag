#version 330 core

in VS2FS
{
	vec3 normal;
	vec3 fragPos;
	vec2 texCoord;
} inBlock;

uniform sampler2D		u_DiffuseTexture;
uniform samplerCube		u_DepthCubemap; 
uniform vec3			u_LightPos;
uniform vec3			u_ViewPos;
uniform float			u_FarPlane;
uniform bool			u_EnableShadow;

uniform samplerCube balda;
uniform sampler2D balda2D;

out vec4			fragColor;

float ShadowCalculation(vec3 fragPos)
{
	vec3 fragToLight = fragPos - u_LightPos;
	float closestDepth;
	float currentDepth = length(fragToLight);
	
	float bias = 0.05;
	float shadow = 0.0;
	float offset = 0.1;
	float samples = 4.0;
	float stride = offset / (samples * 0.5); 

	for(float x = -offset; x < offset; x += stride)
	{
		for(float y = -offset; y < offset; y+= stride)
		{
			for(float z = -offset; z < offset; z+= stride)
			{
				closestDepth = texture(u_DepthCubemap, fragToLight + vec3(x, y, z)).r;
				closestDepth *= u_FarPlane;
				
				if(currentDepth - bias > closestDepth)	shadow += 1.0;
			}
		}
	}

	shadow = shadow  /  (samples * samples * samples);

	//closestDepth *= u_FarPlane;
	//float currentDepth = length(fragToLight);
	//shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	
	return shadow;
}


void main()
{
	vec3 color = texture(u_DiffuseTexture, inBlock.texCoord).rgb;
	vec3 normal = normalize(inBlock.normal);
	vec3 lightColor = vec3(0.3);
	// abmient
	vec3 ambient = 0.3 * color;
	// diffuse
	vec3 lightDir = normalize(u_LightPos - inBlock.fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(u_ViewPos - inBlock.fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max( dot(normal, halfwayDir), 0.0 ), 64.0);
	vec3 specular = spec * lightColor;
	// calculate shadow
	float shadow = u_EnableShadow ? ShadowCalculation(inBlock.fragPos) : 0.0;
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	//vec3 lighting = ambient + (diffuse + specular) * color;
	fragColor = vec4(lighting, 1.0);
}