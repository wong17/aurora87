#version 460 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_SkydomeTex;
uniform float u_Time;
uniform float u_Speed;
uniform float u_VerticalOffset;

void main()
{
	//FragColor = texture(u_SkydomeTex, v_TexCoord);

    float u = fract(v_TexCoord.x + u_Time * u_Speed); // fract devuelve la parte decimal de un numero
    float v = v_TexCoord.y + u_VerticalOffset;
	vec2 uv = vec2(u, v);
    FragColor = texture(u_SkydomeTex, uv);
}