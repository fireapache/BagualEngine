#pragma once

namespace Bagual::Utils
{
	// Based on https://codereview.stackexchange.com/a/173942/222205
	template<typename T>
	class BSingleton 
	{
	public:
		static T& Instance();

		BSingleton(const BSingleton&) = delete;
		BSingleton& operator= (const BSingleton) = delete;

	protected:
		
		BSingleton() {}
	};

	#include <memory>
	template<typename T>
	T& BSingleton<T>::Instance()
	{
		static T instance{ new T{} };
		return *instance;
	}

}