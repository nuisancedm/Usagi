#include <Usagi.h>

#include "Layers/ExampleLayer.h"
#include "Layers/LearnOpenGLLayer.h"
#include "Layers/simplePBRLayer.h"

class Sandbox : public Usagi::Application {
public:
	Sandbox() {
		// PushLayer(new ExampleLayer());
		// PushLayer(new LearnOpenGL());
		PushLayer(new simplePBRLayer());
	}
	~Sandbox() {}

};

Usagi::Application* Usagi::CreateApplication() {
	return new Sandbox();
}