#version 330 core
out vec4 FragColor;

in VS2FS
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} inBlock;

uniform sampler2D u_DiffuseMap;
uniform sampler2D u_NormalMap;

void main()
{	
	vec3 normal = texture(u_NormalMap, inBlock.TexCoord).rgb;
	// transform normal vector to range [-1, 1]
	normal = normalize(normal * 2.0 - 1.0); 
	// this normal is in tangent space

	// get diffuse color
	vec3 color = texture(u_DiffuseMap, inBlock.TexCoord).rgb;
	// ambient
	vec3 ambient = 0.1 * color;
	// diffuse 
	vec3 lightDir = normalize(inBlock.TangentLightPos - inBlock.TangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;
	// specular
	vec3 viewDir = normalize(inBlock.TangentViewPos - inBlock.TangentFragPos);
	vec3 reflectdir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	vec3 specular = vec3(0.2) * spec;
	FragColor = vec4(ambient + diffuse + specular, 1.0);
}
