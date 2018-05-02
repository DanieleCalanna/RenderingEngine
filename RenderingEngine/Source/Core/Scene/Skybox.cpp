#include "Core/Scene/SkyBox.hpp"
#include "Core/Camera/Camera.hpp"
#include <iostream>
#include "Utils/Png/lodepng.h"
#include "Core/Shaders/SkyBoxShader.hpp"
#include "Core/Shaders/StandardShader.hpp"
#include "Core/Transform.hpp"
#include "glm/ext.hpp"
#include "Core/Window.hpp"
#include "Core/Texture/Texture.hpp"

void SkyBoxComponent::Construct()
{
	float CubePoints[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &CubeVertexArray);
	glBindVertexArray(CubeVertexArray);
	glGenBuffers(1, &CubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, CubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &CubePoints, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	float QuadPoints[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &QuadVertexArray);
	glBindVertexArray(QuadVertexArray);
	glGenBuffers(1, &QuadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), &QuadPoints, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//CubemapTexture = GetCubemapFromEquirectangular("Resources/Textures/HDR/Newport_Loft/Newport_Loft_8k.jpg", 4096);
	//HdrTexture = GetCubemapFromEquirectangular("Resources/Textures/HDR/Newport_Loft/Newport_Loft_Ref.hdr", 4096);

	CubemapTexture = GetCubemapFromEquirectangular("Resources/Textures/HDR/Malibu_Overlook/Malibu_Overlook_8k.jpg", 4096);
	HdrTexture = GetCubemapFromEquirectangular("Resources/Textures/HDR/Malibu_Overlook/Malibu_Overlook_3k.hdr", 4096);

	IrradianceCubemap = GetConvolutedCubemap(HdrTexture, 64);
	PrefilteredCubemap = GetPrefilteredCubemap(HdrTexture, 256);
	BRDFTexture = GetBRDFTexture(512);
	//CubemapTexture = PrefilteredCubemap;
	//CubemapTexture = IrradianceCubemap;
	
	/*
	CreateCubemap("Resources/Textures/CubeMap/NegZ.png",
				  "Resources/Textures/CubeMap/PosZ.png",
				  "Resources/Textures/CubeMap/PosY.png",
				  "Resources/Textures/CubeMap/NegY.png",
				  "Resources/Textures/CubeMap/NegX.png",
				  "Resources/Textures/CubeMap/PosX.png"
	);
	*/
}

void SkyBoxComponent::Start() {}

void SkyBoxComponent::CreateCubemap(const std::string Front, const std::string Back,
									const std::string Top, const std::string Bottom,
									const std::string Left, const std::string Right)
{
	// generate a cube-map texture to hold all the sides
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &CubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);

	// load each image and copy into a side of the cube-map texture
	LoadCubemapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Back);
	LoadCubemapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Front);
	LoadCubemapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Top);
	LoadCubemapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Bottom);
	LoadCubemapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Left);
	LoadCubemapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, Right);
	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glActiveTexture(0);
}


bool SkyBoxComponent::LoadCubemapSide(GLenum SideTarget, const std::string TexturePath)
{
	std::vector<unsigned char> PngImage;
	unsigned Width, Height;
	unsigned PngLoadingError = lodepng::decode(PngImage, Width, Height, TexturePath);

	// If there's an error, display it.
	if (PngLoadingError != 0)
	{
		std::cout << "error " << PngLoadingError << ": " << lodepng_error_text(PngLoadingError) << std::endl;
		exit(1);
	}
	// non-power-of-2 dimensions check
	if ((Width & (Width - 1)) != 0 || (Height & (Height - 1)) != 0)
	{
		std::cout << "WARNING: image " << TexturePath << " is not power-of-2 dimensions" << std::endl;
	}
	glTexImage2D(SideTarget, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &PngImage[0]);
	// copy image data into 'target' side of cube map
	return true;
}

void SkyBoxComponent::Update()
{
	const Camera* ActiveCamera = Camera::GetActiveCamera();
	if (ActiveCamera)
	{
		
		glDepthMask(GL_FALSE);
		SkyBoxShader::GetInstance().Start();
		SkyBoxShader::GetInstance().LoadInt("Cubemap", 0);
		SkyBoxShader::GetInstance().LoadProjectionMatrix(ActiveCamera->GetProjectionMatrix());
		SkyBoxShader::GetInstance().LoadViewMatrix(ActiveCamera->GetViewMatrix());
		SkyBoxShader::GetInstance().LoadTransformationMatrix(Transform(ActiveCamera->GetWorldTransform().Location, glm::vec3(0.0f), glm::vec3(1000.0f)).GetTransformationMatrix());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);

		glBindVertexArray(CubeVertexArray);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glActiveTexture(0);
		SkyBoxShader::GetInstance().Stop();
		glDepthMask(GL_TRUE);
	}
}

GLuint SkyBoxComponent::GetCubemapTextureId() const { return CubemapTexture; }

GLuint SkyBoxComponent::GetIrradianceMapTextureId() const { return IrradianceCubemap; }

GLuint SkyBoxComponent::GetPrefilteredCubemapTextureId() const { return PrefilteredCubemap; }

GLuint SkyBoxComponent::GetBRDFTextureId() const { return BRDFTexture; };


GLuint SkyBoxComponent::GetCubemapFromEquirectangular(std::string EquirectangularPath, GLsizei Size) const //TO-DO Refactoring
{
	// Setup framebuffer
	GLuint CaptureFBO;
	GLuint CaptureRBO;
	glGenFramebuffers(1, &CaptureFBO);
	glGenRenderbuffers(1, &CaptureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Size, Size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, CaptureRBO);

	GLuint EquirectangularTextureId = LoadTexture(EquirectangularPath, GL_RGB16F, GL_RGB, GL_FLOAT);

	// Setup cubemap to render to and attach to framebuffer
	GLuint EnvCubemap;
	glGenTextures(1, &EnvCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, EnvCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, Size, Size, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set up projection and view matrices for capturing data onto the 6 cubemap face directions
	glm::mat4 CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 CaptureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// Convert Equirectangular environment map to cubemap equivalent
	Shader EquirectangularToCubemapShader("Resources/Shaders/EquirectangularToCubemapShader/EquirectangularToCubemapVertex.glsl", "Resources/Shaders/EquirectangularToCubemapShader/EquirectangularToCubemapFragment.glsl");
	EquirectangularToCubemapShader.Refresh();
	EquirectangularToCubemapShader.Start();
	EquirectangularToCubemapShader.LoadInt("EquirectangularMap", 0);
	EquirectangularToCubemapShader.LoadMat4("ProjectionMatrix", CaptureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, EquirectangularTextureId);

	glViewport(0, 0, Size, Size); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		EquirectangularToCubemapShader.LoadMat4("ViewMatrix", CaptureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, EnvCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render Cube
		glBindVertexArray(CubeVertexArray);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	EquirectangularToCubemapShader.Stop();
	glDeleteTextures(1, &EquirectangularTextureId);
	glViewport(0, 0, Window::GetSingletonWindow().GetWidth(), Window::GetSingletonWindow().GetHeight());
	return EnvCubemap;
}




GLuint SkyBoxComponent::GetConvolutedCubemap(GLuint Cubemap, GLsizei Size) const //TO-DO Refactoring
{
	// Setup framebuffer
	GLuint CaptureFBO;
	GLuint CaptureRBO;
	glGenFramebuffers(1, &CaptureFBO);
	glGenRenderbuffers(1, &CaptureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Size, Size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, CaptureRBO);

	GLuint IrradianceMap;
	glGenTextures(1, &IrradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, Size, Size, 0,
					 GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Set up projection and view matrices for capturing data onto the 6 cubemap face directions
	glm::mat4 CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 CaptureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// Convert Equirectangular environment map to cubemap equivalent
	Shader CubemapConvolutionShader("Resources/Shaders/CubemapConvolutionShader/CubemapConvolutionVertex.glsl", "Resources/Shaders/CubemapConvolutionShader/CubemapConvolutionFragment.glsl");
	CubemapConvolutionShader.Refresh();
	CubemapConvolutionShader.Start();
	CubemapConvolutionShader.LoadMat4("ProjectionMatrix", CaptureProjection);
	CubemapConvolutionShader.LoadInt("Cubemap", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap);


	glViewport(0, 0, Size, Size); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		CubemapConvolutionShader.LoadMat4("ViewMatrix", CaptureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, IrradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render Cube
		glBindVertexArray(CubeVertexArray);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CubemapConvolutionShader.Stop();
	glViewport(0, 0, Window::GetSingletonWindow().GetWidth(), Window::GetSingletonWindow().GetHeight());
	return IrradianceMap;
}



GLuint SkyBoxComponent::GetPrefilteredCubemap(GLuint Cubemap, GLsizei Size) const //TO-DO Refactoring
{
	// Setup framebuffer
	GLuint CaptureFBO;
	GLuint CaptureRBO;
	glGenFramebuffers(1, &CaptureFBO);
	glGenRenderbuffers(1, &CaptureRBO);

	//glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, CaptureRBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, CaptureRBO);

	GLuint PrefilterMap;
	glGenTextures(1, &PrefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, PrefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, Size, Size, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Set up projection and view matrices for capturing data onto the 6 cubemap face directions
	glm::mat4 CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 CaptureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// Convert Equirectangular environment map to cubemap equivalent
	Shader PrefilterCubemapShader("Resources/Shaders/PrefilterCubemapShader/PrefilterCubemapVertex.glsl", "Resources/Shaders/PrefilterCubemapShader/PrefilterCubemapFragment.glsl");
	PrefilterCubemapShader.Refresh();
	PrefilterCubemapShader.Start();
	PrefilterCubemapShader.LoadMat4("ProjectionMatrix", CaptureProjection);
	PrefilterCubemapShader.LoadInt("Cubemap", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap);

	glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = (unsigned int) (Size * std::pow(0.5, mip));
		unsigned int mipHeight = (unsigned int)(Size * std::pow(0.5, mip));
		glBindRenderbuffer(GL_RENDERBUFFER, CaptureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		PrefilterCubemapShader.LoadFloat("Roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			PrefilterCubemapShader.LoadMat4("ViewMatrix", CaptureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
								   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, PrefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Render Cube
			glBindVertexArray(CubeVertexArray);
			glEnableVertexAttribArray(0);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDisableVertexAttribArray(0);
			glBindVertexArray(0);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	PrefilterCubemapShader.Stop();
	glViewport(0, 0, Window::GetSingletonWindow().GetWidth(), Window::GetSingletonWindow().GetHeight());
	return PrefilterMap;
}

GLuint SkyBoxComponent::GetBRDFTexture(GLsizei Size) const //TO-DO Refactoring
{
	GLuint BRDFLUTTextureId;
	glGenTextures(1, &BRDFLUTTextureId);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, BRDFLUTTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, Size, Size, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLuint CaptureFBO;
	GLuint CaptureRBO;

	glGenFramebuffers(1, &CaptureFBO);
	glGenRenderbuffers(1, &CaptureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Size, Size);

	glViewport(0, 0, Size, Size);

	Shader BRDFShader("Resources/Shaders/BRDFShader/BRDFVertex.glsl", "Resources/Shaders/BRDFShader/BRDFFragment.glsl");
	BRDFShader.Refresh();
	BRDFShader.Start();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BRDFLUTTextureId, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render Quad
	glBindVertexArray(QuadVertexArray);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	BRDFShader.Stop();
	glViewport(0, 0, Window::GetSingletonWindow().GetWidth(), Window::GetSingletonWindow().GetHeight());
	return BRDFLUTTextureId;
}

