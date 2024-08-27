#include "usgpch.h"
#include "Light.h"

namespace Usagi {
	Light::Light(glm::vec3 position, glm::vec3 color) 
		:m_Position(position), m_Color(color)
	{
		
	}
	Light::Light() {
		m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Color = glm::vec3(1.0f, 1.0f, 1.0f);
	}
}
