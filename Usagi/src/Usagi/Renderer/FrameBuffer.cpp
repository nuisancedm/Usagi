#include "usgpch.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "platform/OpenGL/OpenGLFramebuffer.h"

namespace Usagi {

	Ref<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height, FramebufferFormat format)
	{
		Ref<Framebuffer> result = nullptr;

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::NONE:	USG_CORE_ASSERT(false, "RendererAPI::NONE is not supported yet."); return nullptr;
			case RendererAPI::API::OPENGL:	result = std::make_shared<OpenGLFramebuffer>(width, height, format);
		}
		FramebufferPool::GetGlobal()->Add(result);
		return result;
	}

	FramebufferPool* FramebufferPool::s_Instance = new FramebufferPool;

	FramebufferPool::FramebufferPool(uint32_t maxFBs /* = 32 */)
	{

	}

	FramebufferPool::~FramebufferPool()
	{

	}

	std::weak_ptr<Framebuffer> FramebufferPool::AllocateBuffer()
	{
		// m_Pool.push_back();
		return std::weak_ptr<Framebuffer>();
	}

	void FramebufferPool::Add(Ref<Framebuffer> framebuffer)
	{
		m_Pool.push_back(framebuffer);
	}

}