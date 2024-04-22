#include <Usagi.h>

class ExampleLayer : public Usagi::Layer {
public:
	ExampleLayer()
		:Layer("Example") {}

	void OnUpdate() override {
		USG_INFO("ExampleLayer::Update");
	}

	void OnEvent(Usagi::Event& event) override {
		USG_TRACE("{0}", event);
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