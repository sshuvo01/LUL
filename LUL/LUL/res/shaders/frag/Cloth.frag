#version 330 core
out vec4 FragColor;
//in vec2 TexCoords;
//in vec3 WorldPos;
//in vec3 Normal;

// material parameters
uniform vec3 albedo;
uniform sampler2D albedoMap;
uniform sampler2D roughnessMap;
uniform sampler2D normalMap;
uniform float u_alpha;

// lights

uniform vec3 lightColors[4]; 


in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos[4];
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

const float PI = 3.14159265359;

vec3 Fresnel(vec3 n, vec3 l, vec3 F0)
{
	float ndotl = max(0.0, dot(n, l));
	return F0 + (1.0 - F0) * pow((1.0 - ndotl), 5.0);
}

float ClothNDF(vec3 n, vec3 m, float alpha)
{
	// 
	float kAmp = 0.01;
	//
	float ndotm = max(0.0, dot(n, m));
	float ndotm2 = ndotm * ndotm;
	float alpha2 = alpha * alpha;
	float first = ndotm / (PI * (1.0 + kAmp * alpha2));

	float x = ndotm2 / (alpha2 * (ndotm2 - 1.0));
	float secondNumerator = kAmp * exp(x);
	float xx = 1.0 - ndotm2;
	float secondDenominator = xx * xx;

	return first * (1.0 + secondNumerator / secondDenominator);
}

vec3 ClothDiffuse(vec3 n, vec3 v, vec3 l)
{
	//
	vec3 rho = albedo;
	rho = texture(albedoMap, fs_in.TexCoords).rgb;
	vec3 F0 = vec3(0.04, 0.04, 0.04);
	float roughness = texture(roughnessMap, fs_in.TexCoords).r;
	float alpha = roughness * roughness;
	//
	vec3 h = normalize(v + l);
	vec3 fresnel = Fresnel(h, l, F0);
	vec3 first = (1.0 - fresnel) * (rho / PI);
	
	float ndotl = max(0.0, dot(n, l));
	float ndotv = max(0.0, dot(n, v));

	vec3 secondNumerator = fresnel * ClothNDF(n, h, alpha);
	float secondDenominator = max(0.0001, 4*(ndotl + ndotv - ndotl*ndotv));

	return first + (secondNumerator / secondDenominator);
}

void main()
{		

	vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    //vec3 N = normalize(Normal);
	vec3 N = normal;
    vec3 V = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(fs_in.TangentLightPos[i] - fs_in.TangentFragPos);
		//normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
        vec3 H = normalize(V + L);
        float distance = length(fs_in.TangentLightPos[i] - fs_in.FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        float NdotL = max(dot(N, L), 0.0);        
		Lo += ClothDiffuse(N, V, L) * radiance * NdotL;
    }   
    
    vec3 ambient = vec3(0.03) * albedo;
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 
    FragColor = vec4(color, 1.0);
}