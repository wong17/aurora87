// Transforma los vertices al espacio de recorte (clip space) desde el punto de vista de la luz

#version 460 core

layout(location = 0) in vec3 a_Position;
// DEBE tener el mismo LAYOUT LOCATION que el vertex shader de instancing
layout(location = 3) in mat4 a_InstanceTransform;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
    // salida en clip space, que luego se transforma a coordenadas de textura para el depth map.
    gl_Position = u_LightSpaceMatrix * u_ModelMatrix * a_InstanceTransform * vec4(a_Position, 1.0);
}