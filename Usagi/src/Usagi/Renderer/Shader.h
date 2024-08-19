#pragma once
#include <string>
namespace Usagi {
	class Shader {
	public:
		virtual ~Shader() = default; //@@ automatic use default destructor

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}