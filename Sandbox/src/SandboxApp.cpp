#include <Usagi.h>

class ExampleLayer : public Usagi::Layer {
public:
	ExampleLayer()
		:Layer("Example") {}

	void OnUpdate() override {
		// USG_INFO("ExampleLayer::Update");
		if (Usagi::Input::IsKeyPressed(USG_KEY_TAB)) {
			USG_TRACE("Tab Key is pressed (polling)");
		}
	}

	void OnEvent(Usagi::Event& event) override {
		// USG_TRACE("{0}", event);
		if (event.GetEventType() == Usagi::EventType::KeyPressed)
		{
			Usagi::KeyPressedEvent& e = (Usagi::KeyPressedEvent&)event;
			if (e.GetKeyCode() == USG_KEY_TAB) {
				USG_TRACE("Tab Key is pressed (event)");
			}
			USG_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};


class Sandbox :public Usagi::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}

};

Usagi::Application* Usagi::CreateApplication() {
	return new Sandbox();
}