#pragma once

#include "GL\glew.h"
#include <string>

class Texture
{
public:

	Texture(std::string FilePath);

	void Activate(GLenum Slot);

	void Deactivate();

private:

	GLuint TextureId;

	GLenum CurrentSlot = 0;

	bool IsActive();

};