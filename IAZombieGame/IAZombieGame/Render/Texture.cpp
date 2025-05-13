#include "Texture.h"

Texture::Texture(const char* image[], GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    // Asigna el tipo de textura a la textura del objeto
    type = texType;

    // Declara el arreglo de texturas
    glGenTextures(3, IDs); // Genera 3 texturas

    for (int i = 0; i < 3; ++i)
    {
        // Lee cada imagen y asigna las texturas
        int widthImg, heightImg, numColCh;
        stbi_set_flip_vertically_on_load(true); // Voltea la imagen para que aparezca de manera correcta
        unsigned char* bytes = stbi_load(image[i], &widthImg, &heightImg, &numColCh, 0);
        if (!bytes) {
            std::cerr << "Error al cargar la imagen: " << image[i] << std::endl;
            continue;
        }
        if (bytes) {
            std::cout << "Imagen cargada correctamente: " << image[i] << std::endl;
            std::cout << "Ancho: " << widthImg << ", Alto: " << heightImg << ", Canales: " << numColCh << std::endl;
        }

        GLenum format = (numColCh == 4) ? GL_RGBA : GL_RGB;
        std::cout << "Formato OpenGL usado: " << ((format == GL_RGBA) ? "GL_RGBA (con alfa)" : "GL_RGB (sin alfa)") << std::endl;

        // Activa la unidad de textura correspondiente
        glActiveTexture(slot + i);  // Para tex0, tex1, tex2 asignamos unidades consecutivas
        glBindTexture(texType, IDs[i]);

        // Configura los filtros y envolturas de la textura
        glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Asigna la imagen a la textura de OpenGL
        glTexImage2D(texType, 0, format, widthImg, heightImg, 0, format, pixelType, bytes);

        glGenerateMipmap(texType); // Genera mipmaps para la textura

        // Libera la memoria de la imagen ya que ya está cargada en la textura
        stbi_image_free(bytes);
    }

    glBindTexture(texType, 0); // Desvincula la textura
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
    shader.Activate();
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    glUniform1i(texUni, unit);
}

void Texture::Bind(int index)
{
    if (index >= 0 && index < 2)
    {
        glBindTexture(type, IDs[index]);
    }
}

void Texture::Unbind()
{
    glBindTexture(type, 0);
}

void Texture::Delete()
{
    glDeleteTextures(3, IDs); // Elimina las tres texturas
}
