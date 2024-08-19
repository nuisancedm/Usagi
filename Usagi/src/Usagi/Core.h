#pragma once
#include <memory>

#ifdef USAGI_PLATFORM_WINDOWS
#if USG_DYNAMIC_LINK
	#ifdef USAGI_BUILD_DLL
		#define USAGI_API __declspec(dllexport)
	#else
		#define USAGI_API __declspec(dllimport)
	#endif

#endif
#define USAGI_API
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
//============== C++ NOTES HERE =================
//@@ std::bind will make a new callable function as a callback function.
//@@ in CPP, member function is different with normal function, it need a signiture call the function.
//@@ std::bind will make a member funciton to be a new callable more like a normal function.

namespace Usagi {
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Ref = std::shared_ptr<T>;
}