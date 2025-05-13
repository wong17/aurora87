#version 330 core

out vec4 FragColor;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform int figureID;
in vec2 TexCoord;
uniform vec4 lightColor;

void main()
{
	
		 if (figureID % 3 == 0)
        FragColor = texture(tex1, TexCoord) * lightColor;  // Múltiplo de 3 -> textura 2
    else if (figureID % 2 == 0)
        FragColor = texture(tex0, TexCoord) * lightColor;  // Múltiplo de 2 -> textura 1
    else
        FragColor = texture(tex2, TexCoord) * lightColor;  // Ninguno de los dos -> textura 0

}