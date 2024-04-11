#pragma once

#ifdef USAGI_PLATFORM_WINDOWS

extern Usagi::Application* Usagi::CreateApplication();

int main(int argc, char** argv) {
	Usagi::Log::Init();

	USG_CORE_WARN("Initialized Log!");
	int a = 5;
	USG_INFO("Hello! Var={0}", a);
	auto app = Usagi::CreateApplication();
	app->Run();
	delete app;
}

#endif