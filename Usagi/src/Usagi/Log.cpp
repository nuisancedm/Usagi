#include "usgpch.h"
#include "Log.h"


namespace Usagi {
	// ��̬��Ա��Ҫ��������
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
		// ʱ��� , logger��, ������Ϣ
		spdlog::set_pattern("%^[%T] %n: %v%$");
		// ����һ������USAGI�Ĳ�ɫlogger ��ֵ��s_CoreLogger
		s_CoreLogger = spdlog::stdout_color_mt("USAGI"); 
		// trace ������ζ�Ž���¼���м������־��Ϣ��������ϸ�ĸ�����Ϣ�������صĴ���
		s_CoreLogger->set_level(spdlog::level::trace);   

		s_ClientLogger = spdlog::stdout_color_mt("APPLICATION");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}