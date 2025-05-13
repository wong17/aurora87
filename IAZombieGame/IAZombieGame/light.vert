#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;

// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 TexCoord;
uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
	gl_Position = camMatrix * model * vec4(aPos, 1.0f);
		// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	TexCoord = aTex;
	// Assigns the normal from the Vertex Data to "Normal"
}