#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Usagi {
	class USAGI_API Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger(){ return s_ClientLogger; }
	
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core Log Macros
#define USG_CORE_FATAL(...) ::Usagi::Log::GetCoreLogger() -> fatal(__VA_ARGS__)
#define USG_CORE_ERROR(...) ::Usagi::Log::GetCoreLogger() -> error(__VA_ARGS__)
#define USG_CORE_WARN(...)  ::Usagi::Log::GetCoreLogger() -> warn(__VA_ARGS__)
#define USG_CORE_INFO(...)  ::Usagi::Log::GetCoreLogger() -> info(__VA_ARGS__)
#define USG_CORE_TRACE(...) ::Usagi::Log::GetCoreLogger() -> trace(__VA_ARGS__)

// Client Log Macros
#define USG_FATAL(...)		::Usagi::Log::GetClientLogger() -> fatal(__VA_ARGS__)
#define USG_ERROR(...)		::Usagi::Log::GetClientLogger() -> error(__VA_ARGS__)
#define USG_WARN(...)		::Usagi::Log::GetClientLogger() -> warn(__VA_ARGS__)
#define USG_INFO(...)		::Usagi::Log::GetClientLogger() -> info(__VA_ARGS__)
#define USG_TRACE(...)		::Usagi::Log::GetClientLogger() -> trace(__VA_ARGS__)