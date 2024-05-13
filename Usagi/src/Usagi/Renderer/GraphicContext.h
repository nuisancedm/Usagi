#pragma once

namespace Usagi 
{
	class GraphicContext 
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}