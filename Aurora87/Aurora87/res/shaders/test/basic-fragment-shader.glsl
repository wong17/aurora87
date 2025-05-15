#version 460 core

in VS_OUT
{
	vec2 v_TexCoord;
	vec3 v_Normal;
	vec3 v_FragPos;
} fs_in;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main()
{
	FragColor = texture(u_Texture, fs_in.v_TexCoord);
}