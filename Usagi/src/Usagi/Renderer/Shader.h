#pragma once
#include <string>
#include <unordered_map>

namespace Usagi {
	class Shader {
	public:
		virtual ~Shader() = default; //@@ automatic use default destructor

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> create(const std::string& filepath);
		static Ref<Shader> create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary {

	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		bool Exists(const std::string& name) const;
		
		Ref<Shader> Get(const std::string& name);




	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}