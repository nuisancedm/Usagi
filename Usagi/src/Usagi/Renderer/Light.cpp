#include "usgpch.h"
#include "Light.h"

namespace Usagi {
	PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float intensity)
		:Light(color, intensity), m_Position(position), m_AttenuationParams(glm::vec3(1.0, 0.0014, 0.000007))
	{

	}

}