#version 460 core

in VS_OUT
{
	vec2 v_TexCoord;
	vec3 v_Normal;
	vec3 v_FragPos;
	vec4 v_FragPosLightSpace;
} fs_in;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_ShadowMap;

uniform bool u_UseGamma;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

const float GAMMA = 2.2;
const float INV_GAMMA = 1.0 / GAMMA;

// Convertir sRGB a espacio lineal
vec3 Linearize(vec3 color) 
{
	return pow(color, vec3(GAMMA));
}

// Convertir espacio lineal a sRGB
vec3 GammaCorrect(vec3 color) 
{
	return pow(color, vec3(INV_GAMMA));
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0) 
	{
		return 0.0;
	}

	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// calculate bias (based on depth map resolution and slope)
	vec3 normal = normalize(fs_in.v_Normal);
	vec3 lightDir = normalize(u_LightPosition - fs_in.v_FragPos);
	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	
	float cosTheta = clamp(dot(normal, lightDir), 0.0, 1.0);
	float bias = clamp(0.005 * tan(acos(cosTheta)), 0.001, 0.01);

	// PCF
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	return shadow;
}

void main()
{
	vec3 albedo = texture(u_Texture, fs_in.v_TexCoord).rgb;
	// si la textura es sRGB, re-encodifícala a lineal
	if (u_UseGamma)
	{
		albedo = Linearize(albedo);
	}

	// ambient
	vec3 ambient = 0.05 * albedo;

	// diffuse
	vec3 normal = normalize(fs_in.v_Normal);
	vec3 lightDir = normalize(u_LightPosition - fs_in.v_FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * albedo;

	// specular (Blinn-Phong)
	vec3 viewDir = normalize(u_ViewPosition - fs_in.v_FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = vec3(0.3) * spec;

	// shadow
	float shadow = ShadowCalculation(fs_in.v_FragPosLightSpace);

	// combinar
	vec3 color = ambient + (1.0 - shadow) * (diffuse + specular);

	// corrección gamma de salida al framebuffer lineal
	if (u_UseGamma)
	{
		color = GammaCorrect(color);
	}

	FragColor = vec4(color, 1.0);
}