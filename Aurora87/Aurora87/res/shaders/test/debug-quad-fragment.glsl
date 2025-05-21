#version 460 core

in vec2 v_TexCoords;
out vec4 FragColor;

uniform sampler2D u_DepthMap;
uniform float u_Near, u_Far;

float LinearizeDepth(float d) 
{
    // opcional: reconstruye lineal (si quieres ver gradiente real)
    float z = d * 2.0 - 1.0;
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));
}

void main() 
{
    float depthValue = texture(u_DepthMap, v_TexCoords).r;
    // opcional: mostrar lineal
    // depthValue = LinearizeDepth(depthValue) / u_Far;
    //float linearDepth = (depthValue - u_Near) / (u_Far - u_Near);
    FragColor = vec4(vec3(depthValue), 1.0);
}
