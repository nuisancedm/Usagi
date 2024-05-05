#pragma once

#ifdef USAGI_PLATFORM_WINDOWS
	#ifdef USAGI_BUILD_DLL
		#define USAGI_API __declspec(dllexport)
	#else
		#define USAGI_API __declspec(dllimport)
	#endif
#else
	#error Usagi only support Windows!
#endif

#ifdef USG_ENABLE_ASSERTS
	#define USG_ASSERT(x, ...){ if(!(x)) { USG_ERROR("Assertion Failed:{0}", __VA_ARGS__); __debugbreak(); } }
	#define USG_CORE_ASSERT(x, ...){ if(!(x)) { USG_CORE_ERROR("Assertion Failed:{0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define USG_ASSERT(x, ...)
	#define USG_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define USG_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)