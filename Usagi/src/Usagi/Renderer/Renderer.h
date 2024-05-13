#pragma once

namespace Usagi {
	enum class RendererAPI {
		NONE = 0,
		OPENGL=1,
	};
	
	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return m_RendererAPI; }
		
	private:
		static RendererAPI m_RendererAPI;
	};
}


