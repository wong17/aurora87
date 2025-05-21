#version 460 core

out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube u_SkyboxTexture;

void main()
{
	FragColor = texture(u_SkyboxTexture, TexCoord);
}