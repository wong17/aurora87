#version 460 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;
in vec4 v_FragPosLightSpace;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_ShadowMap;

layout(std140, binding = 3) uniform TextureBlock {
    bool u_UseGamma;
    bool u_UseShadows;
};

uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

const float GAMMA = 2.2;
const float INV_GAMMA = 1.0 / GAMMA;

// Funciones para conversión gamma
vec3 Linearize(vec3 color) {
    return pow(color, vec3(GAMMA));
}

vec3 GammaCorrect(vec3 color) {
    return pow(color, vec3(INV_GAMMA));
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    if (!u_UseShadows)
        return 0.0;

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_FragPos);

    float cosTheta = clamp(dot(normal, lightDir), 0.0, 1.0);
    float bias = clamp(0.005 * tan(acos(cosTheta)), 0.001, 0.01);

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
    vec3 albedo = texture(u_Texture, v_TexCoord).rgb;
    if (u_UseGamma)
        albedo = Linearize(albedo);

    vec3 ambient = 0.05 * albedo;

    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * albedo;

    vec3 viewDir = normalize(u_ViewPosition - v_FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = vec3(0.3) * spec;

    float shadow = ShadowCalculation(v_FragPosLightSpace);

    vec3 color = ambient + (1.0 - shadow) * (diffuse + specular);

    if (u_UseGamma)
        color = GammaCorrect(color);

    FragColor = vec4(color, 1.0);
}
