#include "usgpch.h"
#include "RenderCommand.h"
#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace Usagi {
	
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}