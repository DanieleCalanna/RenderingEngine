#pragma once

#include "GL\glew.h"
#include <string>

class Tga;

class Texture
{
public:

	Texture(std::string FilePath);
	Texture(Tga info);

	void Activate(GLenum Slot);

	void Deactivate();

private:

	GLuint TextureId;

	GLenum CurrentSlot = 0;

	bool IsActive();

};