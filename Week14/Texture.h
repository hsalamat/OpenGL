#pragma once
#include <string>
#include <GL/glew.h>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName, GLint Format);

    bool Load();

    void Bind(GLenum TextureUnit);

private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    GLint m_format;
};

