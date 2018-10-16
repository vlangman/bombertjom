#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>
#include "../GameEngine/graphics/includes/Shader.hpp"

namespace GameBoi { namespace graphics {
     /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        GLuint TextureID;   // ID handle of the glyph texture
        glm::ivec2 Size;    // Size of glyph
        glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
        GLuint Advance;    // Horizontal offset to advance to next glyph
    };

    class Fonts {
    public:
        Fonts(const char * file);
        ~Fonts();

        void Load(const char * file);
        void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, int width, int height);
    
    private:
        std::map<GLchar, Character> _characters;
        unsigned int _VAO;
        unsigned int _VBO;
        FT_Library _ft;
        FT_Face _face;
        void    Enable();
        void    Disable();
    };
}}