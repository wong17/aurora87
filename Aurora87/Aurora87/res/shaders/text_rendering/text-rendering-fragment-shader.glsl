#version 460 core

in vec2 v_TexCoord;
out vec4 FragColor;

// Cada glifo est� en el canal rojo de la textura (la cargamos en la clase con GL_RED)
uniform sampler2D u_Text;
// Color RGB del texto (sin alpha): le aplicaremos la m�scara alpha del glifo
uniform vec3 u_TextColor;

void main()
{
    // s�lo tomamos el canal rojo de la textura del glifo
    float alpha = texture(u_Text, v_TexCoord).r;
    FragColor = vec4(u_TextColor, alpha);
}
