#pragma once
#include <glm/glm.hpp>

namespace Usagi{

	class Light {
	public:
		Light(const glm::vec3& color, float intensity)
			: m_Color(color), m_Intensity(intensity) {}
		virtual ~Light() = default;

		inline void setColor(const glm::vec3& color) { m_Color = color; }
		inline void setIntensity(float intensity) { m_Intensity = intensity; };

		inline const glm::vec3& getColor() const { return m_Color; }
		inline float getIntensity() const { return m_Intensity; }

	private:
		float m_Intensity;
		glm::vec3 m_Color;
	};

	class DirectionLight : public Light {
	public:
		DirectionLight(const glm::vec3& direction, const glm::vec3& color, float intensity)
			:Light(color, intensity), m_Direction(direction) 
		{}
		inline void setDirection(const glm::vec3& direction) { m_Direction = direction; }
		inline glm::vec3 getDirection() const { return m_Direction; }
	private:
		glm::vec3 m_Direction;
	};

	class PointLight : public Light {
	public:
		PointLight(const glm::vec3& position, const glm::vec3& color, float intensity);
		inline void setPosition(const glm::vec3& direction) { m_Position = direction; }
		inline void setDecayParameter(glm::vec3& parameter) { m_AttenuationParams = parameter; }
		inline glm::vec3 getPosition() const { return m_Position; }
		inline glm::vec3 getAttenuation() const { return m_AttenuationParams; }
		
	private:
		glm::vec3 m_Position;
		glm::vec3 m_AttenuationParams;
	};
}