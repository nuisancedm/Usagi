#pragma once
#include "Usagi/Layer.h"

#include "Usagi/Events/MouseEvent.h"
#include "Usagi/Events/KeyEvent.h"
#include "Usagi/Events/ApplicationEvent.h"


namespace Usagi {
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		void Begin();
		void End();
	private:
		bool m_BlockEvents = true;
	};
}

