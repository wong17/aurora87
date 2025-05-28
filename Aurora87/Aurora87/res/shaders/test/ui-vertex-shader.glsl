#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;
out vec4 v_FragPosLightSpace;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewMatrix;
    mat4 u_ProjectionMatrix;
};

layout(std140, binding = 1) uniform Model {
    mat4 u_ModelMatrix;
};

uniform mat4 u_LightSpaceMatrix;

void main()
{
    v_TexCoord = a_TexCoord;
    v_Normal = a_Normal;
    vec4 worldPos = u_ModelMatrix * vec4(a_Position, 1.0);
    v_FragPos = worldPos.xyz;
    v_FragPosLightSpace = u_LightSpaceMatrix * worldPos;
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * worldPos;
}
