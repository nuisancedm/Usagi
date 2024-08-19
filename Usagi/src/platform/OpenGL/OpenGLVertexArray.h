#pragma once
#include "Usagi/Renderer/VertexArray.h"

namespace Usagi {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() {}

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& IndexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const override { return m_VertexBuffers; };
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; };


	private:
		std::vector<Ref<VertexBuffer>>  m_VertexBuffers;
		Ref<IndexBuffer>  m_IndexBuffer;

		uint32_t m_RendererID;
	};
}