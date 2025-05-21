#version 460 core

layout(location = 0) in vec3 a_Position;

out vec3 TexCoord;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
	TexCoord = a_Position;
	vec4 pos = u_ProjectionMatrix * u_ViewMatrix * vec4(a_Position, 1.0);
	// Elimina la componente de perspectiva, significa que la posición de la cámara no afecta a la proyección
	// de esta forma la coordenada z se convierte en 1.0
	gl_Position = pos.xyww;
}