
#include <iostream>
#include "Texture.h"
using namespace std;

// Texture variables.
GLuint tID;
GLint twidth, theight, tbitDepth;

Texture::Texture(GLenum TextureTarget, const std::string& FileName, GLint Format)
{
    m_textureTarget = TextureTarget;
    m_fileName = FileName;
    m_format = Format;
}


bool Texture::Load()
{
    stbi_set_flip_vertically_on_load(true);

    //filename.c_str() to convert to constant char*
    //bitDepth:how many bit perpixel
    //unsigned char* image = stbi_load("Media/spheremap.png", &twidth, &theight, &tbitDepth, 0);
    unsigned char* image = stbi_load(m_fileName.c_str(), &twidth, &theight, &tbitDepth, 0);
    if (!image) {
        cout << "Unable to load file!" << stbi_failure_reason() << endl;

        exit(0);
        // Could add a return too if you modify init.
    }


    /// @note: all texture objects cannot be available to the shader. 
    /// That's why we have texture units sitting between texture objects and shaders.
    /// Then shaders samples from the texture unit. 
    /// So between draw calls, we can point to a different texture unit.
    /// The above would return a value such as 16 or 32 or above. That is the number of image samplers that your GPU supports in the fragment shader.
    /// 
    int textureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    cout << "The number of my GPU texture units: " << textureUnits;

    //!Generate a handler for texture object
    glGenTextures(1, &m_textureObj);
    //!This tells openGL if the texture object is 1D, 2D, 3D, etc..
    glBindTexture(m_textureTarget, m_textureObj);
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, twidth, theight, 0, m_format, GL_UNSIGNED_BYTE,image);

    	//! Configure the texture state
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}
