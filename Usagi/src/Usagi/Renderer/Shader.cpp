#include "usgpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace Usagi {
	Ref<Shader> Shader::create(const std::string& filepath) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:   USG_CORE_ASSERT(false, "RendererAPI::NONE is not supported yet."); return nullptr;
		case RendererAPI::API::OPENGL: return std::make_shared<OpenGLShader>(filepath);
		}

		USG_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	};
	
	Ref<Shader> Shader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) 
		{
		case RendererAPI::API::NONE:   USG_CORE_ASSERT(false, "RendererAPI::NONE is not supported yet."); return nullptr;
		case RendererAPI::API::OPENGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		USG_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}


	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
		USG_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader) {
		std::string name = shader->GetName();
		Add(name, shader);
	}
	 


	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
		auto shader = Shader::create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
		auto shader = Shader::create(filepath);
		Add(name, shader);
		return shader;
	}

	bool ShaderLibrary::Exists(const std::string& name) const {
		return m_Shaders.find(name) != m_Shaders.end();
	}


	Ref<Shader> ShaderLibrary::Get(const std::string& name) {
		USG_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}
	

}



