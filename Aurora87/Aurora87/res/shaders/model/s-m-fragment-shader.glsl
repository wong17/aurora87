#version 460 core

in VS_OUT{
    vec2 v_TexCoord;
    vec3 v_Normal;
    vec3 v_FragPos;
    vec4 v_FragPosLightSpace;
    mat3 v_TBN;
} fs_in;

out vec4 FragColor;

// constantes de gamma
const float GAMMA = 2.2;
const float INV_GAMMA = 1.0 / GAMMA;

// arrays de texturas por tipo
#define MAX_TEX 1
uniform sampler2D u_texture_diffuse[MAX_TEX];
uniform sampler2D u_texture_specular[MAX_TEX];
uniform sampler2D u_texture_height[MAX_TEX];
uniform sampler2D u_texture_normal[MAX_TEX];
uniform sampler2D u_texture_emissive[MAX_TEX];
uniform sampler2D u_texture_ambient_occlusion[MAX_TEX];
uniform sampler2D u_texture_opacity[MAX_TEX];
uniform sampler2D u_texture_roughness[MAX_TEX];
uniform sampler2D u_texture_metallic[MAX_TEX];
// Para glTF:
uniform sampler2D u_AlbedoMap;              // BaseColor (RGBA)
uniform sampler2D u_MetallicRoughnessMap;   // (R = metallic, G = roughness, B = occlusion opcional)

layout(std140, binding = 3) uniform TextureBlock {
    int u_NumDiffuseTextures;
    int u_NumSpecularTextures;
    int u_NumHeightTextures;
    int u_NumNormalTextures;
    int u_NumEmissiveTextures;
    int u_NumAmbientOcclusionTextures;
    int u_NumOpacityTextures;
    int u_NumRoughnessTextures;
    int u_NumMetallicTextures;

    int u_HasAlbedoMap;
    int u_HasMetallicRoughnessMap;

    // Factores PBR
    vec4  u_BaseColor;
    float u_MetallicFactor;
    float u_RoughnessFactor;

    // Parametros de sombra/gamma
    bool u_UseGamma;
    bool u_UseShadows;
};

#define MAX_DIR_LIGHTS   1
#define MAX_POINT_LIGHTS 1
#define MAX_SPOT_LIGHTS  1

struct DirectionalLight 
{
    vec4 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight 
{
    vec4 position;

    float constant;
    float linear;
    float quadratic;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight 
{
    vec4 position;
    vec4 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

layout(std140, binding = 4) uniform LightBlock {
    int u_NumDirectionalLights;
    int u_NumPointLights;
    int u_NumSpotLights;

    vec4 u_GlobalAmbient;

    DirectionalLight directionalLights[MAX_DIR_LIGHTS];
    PointLight       pointLights[MAX_POINT_LIGHTS];
    SpotLight        spotLights[MAX_SPOT_LIGHTS];
};

// parametros de iluminacion y shadow
uniform sampler2D u_ShadowMap;
uniform vec3 u_ViewPosition;

// Para samplear y promediar un array de texturas
vec4 SampleArray(sampler2D tex[MAX_TEX], int count, vec2 uv);

vec4 GetAlbedo();
vec3 GetNormal();
void GetMetallicRoughness(out float metallic, out float roughness);
float GetAO();
vec3 GetEmissive();
float GetOpacity(vec4 baseTex);

// Convertir sRGB -> lineal
vec3 Linearize(vec3 c);
// Convertir lineal -> sRGB
vec3 GammaCorrect(vec3 c);

// ShadowMapping con PCF
float ShadowCalculation(vec4 fragPosLightSpace);

//
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 ComputeLighting(vec3 albedo, vec3 N, vec3 emissive, float ao);

void main() 
{
    // 1) BaseColor + alpha
    vec4 baseTex = GetAlbedo();
    vec3 albedo = baseTex.rgb;

    // 1a) Linealizacion (si corresponde)
    if (u_UseGamma)
        albedo = Linearize(albedo);

    // 2) Normal (con o sin normal map)
    vec3 N = GetNormal();

    // 3) Metallic + Roughness
    float metallic, roughness;
    GetMetallicRoughness(metallic, roughness);

    // 4) Ambient Occlusion
    float ao = GetAO();

    // 5) Emissivo
    vec3 emissive = GetEmissive();

    // 6) Opacity / Alpha clip (si corresponde)
    float alpha = GetOpacity(baseTex);

    // 7) Iluminación Blinn‐Phong + Sombras
    vec3 lighting = u_GlobalAmbient.xyz * albedo * ao;
    lighting += ComputeLighting(albedo, N, emissive, ao);

    // 8) Corrección gamma final
    if (u_UseGamma)
        lighting = GammaCorrect(lighting);

    FragColor = vec4(lighting, alpha);
}

// utilidad para samplear y promediar un array de texturas
vec4 SampleArray(sampler2D tex[MAX_TEX], int count, vec2 uv) 
{
    if (count == 0) 
        return vec4(0.0);
    
    vec4 sum = vec4(0.0);
    for (int i = 0; i < count; ++i) 
    {
        sum += texture(tex[i], uv);
    }

    return sum / float(count);
}

vec4 GetAlbedo() 
{
    if (u_HasAlbedoMap == 1) 
    {
        vec4 baseTex = texture(u_AlbedoMap, fs_in.v_TexCoord);
        if (baseTex.a > 0.1) 
        {
            return baseTex;
        }

        if (u_NumDiffuseTextures > 0) 
        {
            return SampleArray(u_texture_diffuse, u_NumDiffuseTextures, fs_in.v_TexCoord);
        }
    }
    else 
    {
        if (u_NumDiffuseTextures > 0) 
        {
            return SampleArray(u_texture_diffuse, u_NumDiffuseTextures, fs_in.v_TexCoord);
        }
    }

    return u_BaseColor;
}

vec3 GetNormal() 
{
    vec3 N = normalize(fs_in.v_Normal);
    if (u_NumNormalTextures > 0) 
    {
        vec3 sampled = texture(u_texture_normal[0], fs_in.v_TexCoord).rgb;
        sampled = normalize(sampled * 2.0 - 1.0); // [0,1]->[-1,1]
        N = normalize(fs_in.v_TBN * sampled);
    }
    return N;
}

void GetMetallicRoughness(out float metallic, out float roughness) 
{
    if (u_HasMetallicRoughnessMap == 1) 
    {
        vec4 mr = texture(u_MetallicRoughnessMap, fs_in.v_TexCoord);
        metallic = mr.r;
        roughness = mr.g;
        return;
    }

    metallic = u_MetallicFactor;
    roughness = u_RoughnessFactor;

    bool hasSeparateMetal = (u_NumMetallicTextures > 0);
    bool hasSeparateRgh = (u_NumRoughnessTextures > 0);

    if (!hasSeparateMetal && !hasSeparateRgh) 
    {
        vec4 mr = texture(u_MetallicRoughnessMap, fs_in.v_TexCoord);
        metallic = mr.r;
        roughness = mr.g;
        return;
    }

    if (hasSeparateMetal) 
    {
        metallic = texture(u_texture_metallic[0], fs_in.v_TexCoord).r * u_MetallicFactor;
    }
    if (hasSeparateRgh) 
    {
        roughness = texture(u_texture_roughness[0], fs_in.v_TexCoord).r * u_RoughnessFactor;
    }
}

float GetAO() 
{
    if (u_NumAmbientOcclusionTextures > 0)
        return texture(u_texture_ambient_occlusion[0], fs_in.v_TexCoord).r;
    return 1.0;
}

vec3 GetEmissive() 
{
    if (u_NumEmissiveTextures > 0)
        return texture(u_texture_emissive[0], fs_in.v_TexCoord).rgb;
    return vec3(0.0);
}

float GetOpacity(vec4 baseTex) 
{
    float alpha = 1.0;
    if (u_NumOpacityTextures > 0) 
    {
        alpha = texture(u_texture_opacity[0], fs_in.v_TexCoord).r;
    }
    else
    {
        alpha = baseTex.a;
    }

    // clip if almost transparent
    if (alpha < 0.1)
        discard;

    return alpha;
}

// convertir sRGB -> lineal
vec3 Linearize(vec3 c) 
{
    return pow(c, vec3(GAMMA));
}
// lineal -> sRGB
vec3 GammaCorrect(vec3 c) 
{
    return pow(c, vec3(INV_GAMMA));
}

// ShadowMapping con PCF
float ShadowCalculation(vec4 fragPosLightSpace) 
{
    if (!u_UseShadows)
        return 0.0;

    vec3 proj = fragPosLightSpace.xyz / fragPosLightSpace.w;
    proj = proj * 0.5 + 0.5;
    if (proj.x < 0.0 || proj.x > 1.0 || proj.y < 0.0 || proj.y > 1.0 || proj.z > 1.0)
        return 0.0;

    float closestDepth  = texture(u_ShadowMap, proj.xy).r;
    float currentDepth  = proj.z;

    vec3 N              = normalize(fs_in.v_Normal);
    vec3 L              = normalize(-directionalLights[0].direction.xyz);
    float cosNL         = clamp(dot(N, L), 0.0, 1.0);
    float bias          = clamp(0.005 * tan(acos(cosNL)), 0.001, 0.01);

    float shadow    = 0.0;
    vec2 texelSize  = 1.0 / textureSize(u_ShadowMap, 0);
    for (int x = -1; x <= 1; ++x) 
    {
        for (int y = -1; y <= 1; ++y) 
        {
            float pcfDepth = texture(u_ShadowMap, proj.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias > pcfDepth ? 1.0 : 0.0);
        }
    }
    return shadow / 9.0;
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    // ambient
    vec3 ambient = light.ambient.xyz;
    // diffuse
    vec3 L = normalize(-light.direction.xyz);
    float diff = max(dot(normal, L), 0.0);
    vec3 diffuse = light.diffuse.xyz * diff;
    // specular
    vec3 halfway = normalize(L + viewDir);
    float spec = pow(max(dot(normal, halfway), 0.0), 64.0);
    vec3 specular = light.specular.xyz * spec;

    return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // ambient
    vec3 ambient = light.ambient.xyz;
    // diffuse
    vec3 L = normalize(light.position.xyz - fragPos);
    float diff = max(dot(normal, L), 0.0);
    vec3 diffuse = light.diffuse.xyz * diff;
    // specular
    vec3 halfway = normalize(L + viewDir);
    float spec = pow(max(dot(normal, halfway), 0.0), 64.0);
    vec3 specular = light.specular.xyz * spec;
    // attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // ambient
    vec3 ambient = light.ambient.xyz;
    // diffuse
    vec3 L = normalize(light.position.xyz - fragPos);
    float diff = max(dot(normal, L), 0.0);
    vec3 diffuse = light.diffuse.xyz * diff;
    // specular
    vec3 halfway = normalize(L + viewDir);
    float spec = pow(max(dot(normal, halfway), 0.0), 64.0);
    vec3 specular = light.specular.xyz * spec;
    // attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight effect
    float theta = dot(L, normalize(-light.direction.xyz));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    return (ambient + diffuse + specular) * attenuation * intensity;
}

vec3 ComputeLighting(vec3 albedo, vec3 N, vec3 emissive, float ao) 
{
    vec3 result = vec3(0.0);
    vec3 viewDir = normalize(u_ViewPosition - fs_in.v_FragPos);

    // 1) Directional Lights
    for (int i = 0; i < u_NumDirectionalLights; ++i)
    {
        result += CalculateDirectionalLight(directionalLights[i], N, viewDir) * albedo;
    }

    // 2) Point Lights
    for (int i = 0; i < u_NumPointLights; ++i)
    {
        result += CalculatePointLight(pointLights[i], N, fs_in.v_FragPos, viewDir) * albedo;
    }

    // 3) Spot Lights
    for (int i = 0; i < u_NumSpotLights; ++i)
    {
        result += CalculateSpotLight(spotLights[i], N, fs_in.v_FragPos, viewDir) * albedo;
    }

    vec3 ambientSum = vec3(0.0);
    for (int i = 0; i < u_NumDirectionalLights; ++i)
    {
        ambientSum += directionalLights[i].ambient.xyz * albedo;
    }
    for (int i = 0; i < u_NumPointLights; ++i)
    {
        ambientSum += pointLights[i].ambient.xyz * albedo;
    }
    for (int i = 0; i < u_NumSpotLights; ++i)
    {
        ambientSum += spotLights[i].ambient.xyz * albedo;
    }

    vec3 diffSpecSum = result - ambientSum;
    float shadow = ShadowCalculation(fs_in.v_FragPosLightSpace);
    vec3 color = ambientSum * ao + (1.0 - shadow) * diffSpecSum + emissive;

    return color;
}