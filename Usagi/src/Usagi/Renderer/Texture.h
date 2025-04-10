#pragma once
#include <string>
#include "Usagi/Core.h"

namespace Usagi {
	class Texture {
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const std::string& filepath);
	};

	class TextureCube : public Texture
	{
	public:
		static Ref<TextureCube> Create(const std::string& filepath);
		static Ref<TextureCube> Create(const std::vector<std::string>& filepaths);
	};
}