#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

out VS_OUT
{
	vec2 v_TexCoord;
	vec3 v_Normal;
	vec3 v_FragPos;
} vs_out;

layout(std140, binding = 0) uniform Camera {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
};

layout(std140, binding = 1) uniform Model {
	mat4 u_ModelMatrix;
};

void main()
{
	vs_out.v_TexCoord = a_TexCoord;
	//vs_out.v_Normal = a_Normal;
	vs_out.v_Normal = normalize(mat3(transpose(inverse(u_ModelMatrix))) * a_Normal);
	vs_out.v_FragPos = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
}