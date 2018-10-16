#include "../includes/Textures.hpp"
#include "../../../build/_deps/stb_image-src/stb_image.h"

namespace GameBoi { namespace graphics {
    Texture::Texture(const std::string& filename) : m_FileName(filename){
        m_TID = load();
    }

    GLuint  Texture::load() {
        unsigned int texture;
		glGenTextures(1, &texture);
		
		// load and generate the texture
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(m_FileName.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format = 0;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(data);
		return texture;
    }

    void	Texture::bind() const {
		glBindTexture(GL_TEXTURE_2D, m_TID);
	}

}}