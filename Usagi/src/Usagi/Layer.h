#pragma once

#include "Usagi/Core.h"
#include "Usagi/Events/Event.h"
#include "Usagi/Core/Timestep.h"

namespace Usagi {
	
	class USAGI_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");

		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}


