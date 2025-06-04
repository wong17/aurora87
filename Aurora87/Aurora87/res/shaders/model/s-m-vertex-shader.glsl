#version 460 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec3  a_Normal;
layout(location = 2) in vec2  a_TexCoord;
layout(location = 3) in vec3  a_Tangent;
layout(location = 4) in vec3  a_Bitangent;
layout(location = 5) in mat4  a_InstanceTransform;

// Para skinning
layout(location = 9) in ivec4 a_BoneIDs;
layout(location = 10) in vec4 a_Weights;

out VS_OUT{
    vec2 v_TexCoord;
    vec3 v_Normal;
    vec3 v_FragPos;
    vec4 v_FragPosLightSpace;
    mat3 v_TBN; // (Tangent-Bitangent-Normal)
} vs_out;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewMatrix;
    mat4 u_ProjectionMatrix;
};

layout(std140, binding = 1) uniform Model {
    mat4 u_ModelMatrix;
};

layout(std140, binding = 2) uniform Bones {
    mat4 u_Bones[100];        // Le damos 100 al maximo numero de huesos
};

uniform bool u_HasAnimation;
uniform int  u_NumBones;

uniform mat4 u_LightSpaceMatrix;

void main()
{
    // UV
    vs_out.v_TexCoord = a_TexCoord;

    // Skinning: calculamos la matriz de piel (si hay animacion)
    mat4 skinMatrix = mat4(1.0);
    if (u_HasAnimation)
    {
        skinMatrix =
            a_Weights.x * u_Bones[a_BoneIDs.x] +
            a_Weights.y * u_Bones[a_BoneIDs.y] +
            a_Weights.z * u_Bones[a_BoneIDs.z] +
            a_Weights.w * u_Bones[a_BoneIDs.w];
    }

    // Aplicamos skinning a posicion y normal (y a Tangent/Bitangent)
    vec4 skinnedPos = skinMatrix * vec4(a_Position, 1.0);
    vec3 skinnedNorm = mat3(skinMatrix) * a_Normal;
    vec3 skinnedTangent = mat3(skinMatrix) * a_Tangent;
    vec3 skinnedBitan = mat3(skinMatrix) * a_Bitangent;

    // Instancing + modelo
    vec4 worldPos = u_ModelMatrix * a_InstanceTransform * skinnedPos;
    vs_out.v_FragPos = worldPos.xyz;

    // Normales en espacio mundo
    mat3 normalMatrix = transpose(inverse(mat3(u_ModelMatrix * a_InstanceTransform)));
    vs_out.v_Normal = normalize(normalMatrix * skinnedNorm);

    // Calculamos también el Tangent y Bitangent en espacio mundo
    // multiplicamos skinnedTangent/Bitan por la misma matriz 3×3 que usamos en la normal
    vec3 T_world = normalize(normalMatrix * skinnedTangent);
    vec3 B_world = normalize(normalMatrix * skinnedBitan);
    vec3 N_world = vs_out.v_Normal;

    // Armamos la matriz TBN
    vs_out.v_TBN = mat3(T_world, B_world, N_world);

    // Light-space for shadow mapping
    vs_out.v_FragPosLightSpace = u_LightSpaceMatrix * vec4(vs_out.v_FragPos, 1.0);

    // Proyección final
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * worldPos;
}
