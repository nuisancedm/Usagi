#pragma once

#ifdef USAGI_PLATFORM_WINDOWS

extern Usagi::Application* Usagi::CreateApplication();

int main(int argc, char** argv) {
	// 初始化Logeger
	Usagi::Log::Init();
	USG_CORE_WARN("Initialized Log!");

	// 初始化Application，Application会创建Window
	auto app = Usagi::CreateApplication();
	app->Run();
	delete app;
}

#endif