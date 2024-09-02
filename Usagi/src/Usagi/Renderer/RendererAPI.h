#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Usagi {

	using RendererID = uint32_t;

	class RendererAPI {
	public:
		enum class API {
			NONE = 0,
			OPENGL = 1,
		};
		enum DepthFnType {
			ALWAYS, NEVER, LESS, EQUAL, LEQUAL, GREATER, NOTEQUAL, GEQUAL
		};

		struct RenderAPICapabilities
		{
			std::string Vendor;
			std::string Renderer;
			std::string Version;

			int MaxSamples;
			float MaxAnisotropy;
		};

	public:
		virtual void Init() = 0;
		
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, bool enableDepthTesting) = 0;
		virtual void SetDepthMask(bool flag) = 0;
		virtual void SetDepthFn(DepthFnType flag) = 0;

		static RenderAPICapabilities& GetCapabilities()
		{
			static RenderAPICapabilities capabilities;
			return capabilities;
		}

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}