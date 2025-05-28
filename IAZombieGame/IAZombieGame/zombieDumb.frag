#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 TexCoord;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the current position from the Vertex Shader
in vec3 crntPos;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;
uniform int figureID;

void main()
{
	// ambient lighting
	float ambient = 0.5f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	 if (figureID % 3 == 0)
        FragColor = texture(tex1, TexCoord) * lightColor * (diffuse + ambient + specular);  // Múltiplo de 3 -> textura 2
    else if (figureID % 2 == 0)
        FragColor = texture(tex0, TexCoord)* lightColor * (diffuse + ambient + specular);  // Múltiplo de 2 -> textura 1
    else
        FragColor = texture(tex2, TexCoord)* lightColor * (diffuse + ambient + specular);  // Ninguno de los dos -> textura 0

}