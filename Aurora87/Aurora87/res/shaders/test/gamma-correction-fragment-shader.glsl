#version 460 core

in VS_OUT
{
	vec2 v_TexCoord;
	vec3 v_Normal;
	vec3 v_FragPos;
} fs_in;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform bool u_UseGamma;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

vec3 applyGamma(vec3 color, float gamma)
{
	return pow(color, vec3(1.0 / gamma));
}

void main()
{
	vec3 albedo = texture(u_Texture, fs_in.v_TexCoord).rgb;

	// si la textura es sRGB, re-encodifícala a lineal
	if (u_UseGamma)
	{
		albedo = applyGamma(albedo, 2.2);
	}

	// ambient
	vec3 ambient = 0.05 * albedo;

	// diffuse
	vec3 lightDir = normalize(u_LightPosition - fs_in.v_FragPos);
	vec3 normal = normalize(fs_in.v_Normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * albedo;

	// specular
	vec3 viewDir = normalize(u_ViewPosition - fs_in.v_FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;

	// blinn
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	vec3 specular = vec3(0.3) * spec;

	vec3 color = ambient + diffuse + specular;

	// corrección gamma de salida al framebuffer lineal
	if (u_UseGamma)
	{
		color = applyGamma(color, 2.2);
	}

	FragColor = vec4(color, 1.0);
}