#version 460 core

out vec4 FragColor;

in VS_OUT
{
    vec2 v_TexCoord;
    vec3 v_Normal;
    vec3 v_FragPos;
} fs_in;

#define MAX_TEXTURES_PER_TYPE 7

// Arrays de texturas
uniform sampler2D u_texture_diffuse[MAX_TEXTURES_PER_TYPE];
uniform sampler2D u_texture_specular[MAX_TEXTURES_PER_TYPE];
uniform sampler2D u_texture_height[MAX_TEXTURES_PER_TYPE];
uniform sampler2D u_texture_normal[MAX_TEXTURES_PER_TYPE];
uniform sampler2D u_texture_emissive[MAX_TEXTURES_PER_TYPE];
uniform sampler2D u_texture_ambient_occlusion[MAX_TEXTURES_PER_TYPE];
uniform sampler2D u_texture_opacity[MAX_TEXTURES_PER_TYPE];
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_MetallicRoughnessMap;
uniform sampler2D u_texture_roughness[MAX_TEXTURES_PER_TYPE];
uniform sampler2D u_texture_metallic[MAX_TEXTURES_PER_TYPE];
// Contadores de texturas activas
uniform int u_NumDiffuseTextures;
uniform int u_NumSpecularTextures;
uniform int u_NumHeightTextures;
uniform int u_NumNormalTextures;
uniform int u_NumEmissiveTextures;
uniform int u_NumAmbientOcclusionTextures;
uniform int u_NumOpacityTextures;
uniform int u_NumRoughnessTextures;
uniform int u_NumMetallicTextures;
// Factores base
uniform vec3 u_BaseColor;
uniform float u_MetallicFactor;
uniform float u_RoughnessFactor;

// Función para combinar múltiples texturas
vec4 sample_texture_array(sampler2D texArray[MAX_TEXTURES_PER_TYPE], int count) {
    if (count == 0) return vec4(1.0); // Dejarlo en blanco por defecto

    vec4 result = vec4(0.0);
    for (int i = 0; i < count; i++) {
        result += texture(texArray[i], fs_in.v_TexCoord);
    }
    return result / count;
}

void main()
{
    // Obtener color base
    vec4 base = sample_texture_array(u_texture_diffuse, u_NumDiffuseTextures);
    base.rgb *= u_BaseColor; // Mezclar con color uniforme

    // Obtener valores PBR
    float metallic = u_MetallicFactor;
    if (u_NumMetallicTextures > 0) {
        metallic *= texture(u_texture_metallic[0], fs_in.v_TexCoord).r;
    }

    float roughness = u_RoughnessFactor;
    if (u_NumRoughnessTextures > 0) {
        roughness *= texture(u_texture_roughness[0], fs_in.v_TexCoord).r;
    }

    // Aplicar otros efectos (ejemplo simple)
    vec4 emission = sample_texture_array(u_texture_emissive, u_NumEmissiveTextures);
    vec4 ao = sample_texture_array(u_texture_ambient_occlusion, u_NumAmbientOcclusionTextures);

    // Mezcla final (aquí irían tus cálculos de iluminación)
    FragColor = base * ao + emission;
    FragColor.a = sample_texture_array(u_texture_opacity, u_NumOpacityTextures).r;

    // Si no hay texturas de ningún tipo, usar color base
    if (u_NumDiffuseTextures == 0 && u_NumOpacityTextures == 0) {
        FragColor = vec4(u_BaseColor, 1.0);
    }
}