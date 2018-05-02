#pragma once

#include "GL\glew.h"
#include <string>

class Tga;

class Texture
{
public:

	Texture(std::string FilePath, GLint InternalFormat, GLenum PixelFormat, GLenum PixelType);
	Texture(std::string FilePath);
	Texture(Tga info);

	void Activate(GLenum Slot);

	void Deactivate();

private:

	GLuint TextureId;

	GLenum CurrentSlot = 0;

	bool IsActive();

};

GLuint LoadTexture(std::string FilePath, GLint InternalFormat, GLenum PixelFormat, GLenum PixelType);
GLuint LoadTexture2(std::string FilePath, GLint InternalFormat, GLenum PixelFormat, GLenum PixelType);

