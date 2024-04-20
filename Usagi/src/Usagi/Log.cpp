#include "usgpch.h"
#include "Log.h"


namespace Usagi {
	// 静态成员需要单独定义
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
		// 时间戳 , logger名, 具体信息
		spdlog::set_pattern("%^[%T] %n: %v%$");
		// 创建一个名叫USAGI的彩色logger 赋值给s_CoreLogger
		s_CoreLogger = spdlog::stdout_color_mt("USAGI"); 
		// trace 级别意味着将记录所有级别的日志消息，从最详细的跟踪信息到最严重的错误。
		s_CoreLogger->set_level(spdlog::level::trace);   

		s_ClientLogger = spdlog::stdout_color_mt("APPLICATION");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}