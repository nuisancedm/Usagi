#pragma once
#include "Usagi/Renderer/Texture.h"

namespace Usagi {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(const std::vector<std::string>& paths);

		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; };
		virtual uint32_t GetHeight() const override { return m_Height; };

		virtual void Bind(uint32_t slot) const override;
	
	private:
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
	};

	class OpenGLTextureCube : public TextureCube {
	public:
		OpenGLTextureCube(const std::string& path);
		OpenGLTextureCube(const std::vector<std::string>& paths);
		virtual ~OpenGLTextureCube();

		virtual uint32_t GetWidth() const override { return m_Width; };
		virtual uint32_t GetHeight() const override { return m_Height; };

		virtual void Bind(uint32_t slot) const override;

	private:
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
	};
}