#include "Core/Texture/Texture.hpp"
#include "Utils/Tga.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

Texture::Texture(std::string FilePath)
{
	Tga info = Tga(FilePath.c_str());

	glGenerateMipmap(GL_TEXTURE_2D);
	glGenTextures(1, &TextureId);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, info.GetWidth(), info.GetWidth(), 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, info.GetPixels().data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(std::string FilePath, GLint InternalFormat, GLenum PixelFormat, GLenum PixelType)
{
	TextureId = LoadTexture2(FilePath, InternalFormat, PixelFormat, PixelType);
}

Texture::Texture(Tga info)
{
	glGenerateMipmap(GL_TEXTURE_2D);
	glGenTextures(1, &TextureId);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, info.GetWidth(), info.GetWidth(), 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, info.GetPixels().data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Activate(GLenum Slot)
{
	//if (IsActive()) Deactivate();
	CurrentSlot = Slot;
	glActiveTexture(CurrentSlot);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	//glActiveTexture(0);
}

void Texture::Deactivate()
{
	glActiveTexture(CurrentSlot);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glActiveTexture(0);
	CurrentSlot = 0;
}

bool Texture::IsActive()
{
	return CurrentSlot != 0;
}


GLuint LoadTexture(std::string FilePath, GLint InternalFormat, GLenum PixelFormat, GLenum PixelType) //TO-DO Refactoring
{
	{
		// Load the Equirectangular environment map
		stbi_set_flip_vertically_on_load(true);
		int Width, Height, Components;
		void *Data = stbi_loadf(FilePath.c_str(), &Width, &Height, &Components, 0);
		GLuint TextureId;
		if (Data)
		{
			glGenTextures(1, &TextureId);
			glBindTexture(GL_TEXTURE_2D, TextureId);
			glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, PixelFormat, PixelType, Data); // note how we specify the texture's data value to be float

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(Data);
			return TextureId;
		}
		else
		{
			return 0;
		}
	}
}

GLuint LoadTexture2(std::string FilePath, GLint InternalFormat, GLenum PixelFormat, GLenum PixelType) //TO-DO Refactoring
{
	{
		// Load the Equirectangular environment map
		stbi_set_flip_vertically_on_load(true);
		int Width, Height, Components;
		void *Data = stbi_load(FilePath.c_str(), &Width, &Height, &Components, 3);
		GLuint TextureId;
		if (Data)
		{
			glGenTextures(1, &TextureId);
			glBindTexture(GL_TEXTURE_2D, TextureId);
			glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, PixelFormat, PixelType, Data); // note how we specify the texture's data value to be float

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(Data);
			return TextureId;
		}
		else
		{
			return 0;
		}
	}
}


GLuint LoadTextureFont(std::string FilePath, GLint InternalFormat, GLenum PixelFormat, GLenum PixelType) //TO-DO Refactoring
{
	// Load the Equirectangular environment map
	stbi_set_flip_vertically_on_load(true);
	int Width, Height, Components;
	void *Data = stbi_load(FilePath.c_str(), &Width, &Height, &Components, 3);
	GLuint TextureId;
	if (Data)
	{
		glGenTextures(1, &TextureId);
		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, PixelFormat, PixelType, Data); // note how we specify the texture's data value to be float

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(Data);
		return TextureId;
	}
	else
	{
		return 0;
	}

}