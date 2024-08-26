#include <Usagi.h>

#include "Layers/ExampleLayer.h"
#include "Layers/LearnOpenGLLayer.h"

class Sandbox : public Usagi::Application {
public:
	Sandbox() {
		// PushLayer(new ExampleLayer());
		PushLayer(new LearnOpenGL());
	}
	~Sandbox() {}

};

Usagi::Application* Usagi::CreateApplication() {
	return new Sandbox();
}