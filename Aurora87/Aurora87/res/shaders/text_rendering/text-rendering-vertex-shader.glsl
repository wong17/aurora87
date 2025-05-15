#version 460 core

// Cada v�rtice lleva: 
//  - a_Vertex.xy = posici�n en pantalla (en unidades de p�xeles) 
//  - a_Vertex.zw = coordenadas de textura (0�1)
layout(location = 0) in vec4 a_Vertex;

out vec2 v_TexCoord;

// Matriz de proyecci�n ortogr�fica 
uniform mat4 u_ProjectionMatrix;

void main()
{
    // Convertimos la posici�n en pixel space a clip space
    gl_Position = u_ProjectionMatrix * vec4(a_Vertex.xy, 0.0, 1.0);
    v_TexCoord = a_Vertex.zw;
}
