#include "usgpch.h"
#include "OpenGLTexture.h"
#include "Usagi/Renderer/RendererAPI.h"

#include "stb_image.h"
#include <glad/glad.h>

#include <iostream>

namespace Usagi {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{	
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		USG_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0;
		GLenum dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 1) {
			internalFormat = GL_R8;
			dataFormat = GL_RED;
		}

		USG_CORE_ASSERT(internalFormat & dataFormat, "image format not supported")

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID);
	}


	OpenGLTextureCube::OpenGLTextureCube(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		m_Width = width;
		m_Height = height;

		unsigned int faceWidth = m_Width / 4;
		unsigned int faceHeight = m_Height / 3;
		USG_CORE_ASSERT(faceWidth == faceHeight, "Non-square faces!");

		std::array<unsigned char*, 6> faces;
		for (size_t i = 0; i < faces.size(); i++)
			faces[i] = new unsigned char[faceWidth * faceHeight * 3]; // 3 BPP

		int faceIndex = 0;

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + i * faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = data[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = data[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = data[(xOffset + yOffset * m_Width) * 3 + 2];
				}
			}
			faceIndex++;
		}

		for (size_t i = 0; i < 3; i++)
		{
			// Skip the middle one
			if (i == 1)
				continue;

			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + i * faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = data[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = data[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = data[(xOffset + yOffset * m_Width) * 3 + 2];
				}
			}
			faceIndex++;
		}

		GLenum internalFormat = 0;
		GLenum dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 1) {
			internalFormat = GL_R8;
			dataFormat = GL_RED;
		}


		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameterf(m_RendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[2]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[0]);
																								  
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[5]);
																								  
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, faceWidth, faceHeight, 0, dataFormat, GL_UNSIGNED_BYTE, faces[3]);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		for (size_t i = 0; i < faces.size(); i++)
			delete[] faces[i];

		stbi_image_free(data);
	}

	OpenGLTextureCube::OpenGLTextureCube(const std::vector<std::string>& paths) {
		
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		stbi_set_flip_vertically_on_load(false);

		int width, height, channels;
		for (unsigned int i = 0; i < paths.size(); i++)
		{
			unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);

			GLenum internalFormat = 0;
			GLenum dataFormat = 0;
			if (channels == 4) {
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3) {
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (channels == 1) {
				internalFormat = GL_R8;
				dataFormat = GL_RED;
			}

			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << paths[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTextureParameterf(m_RendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	OpenGLTextureCube::~OpenGLTextureCube() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureCube::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID);
	}
}