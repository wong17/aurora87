#version 460 core

// Cada vértice lleva: 
//  - a_Vertex.xy = posición en pantalla (en unidades de píxeles) 
//  - a_Vertex.zw = coordenadas de textura (0–1)
layout(location = 0) in vec4 a_Vertex;

out vec2 v_TexCoord;

// Matriz de proyección ortográfica 
uniform mat4 u_ProjectionMatrix;

void main()
{
    // Convertimos la posición en pixel space a clip space
    gl_Position = u_ProjectionMatrix * vec4(a_Vertex.xy, 0.0, 1.0);
    v_TexCoord = a_Vertex.zw;
}
