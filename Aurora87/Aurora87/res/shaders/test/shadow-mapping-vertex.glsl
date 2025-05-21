#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out VS_OUT
{
	vec2 v_TexCoord;
	vec3 v_Normal;
	vec3 v_FragPos;
	vec4 v_FragPosLightSpace;
} vs_out;

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
	vs_out.v_TexCoord = a_TexCoord;
	vs_out.v_Normal = a_Normal;
	vs_out.v_FragPos = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
	vs_out.v_FragPosLightSpace = u_LightSpaceMatrix * vec4(vs_out.v_FragPos, 1.0);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
}