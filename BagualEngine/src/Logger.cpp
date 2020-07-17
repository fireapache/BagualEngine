
#include "Bagual.pch.h"

#include "Logger.h"

#include <iostream>

namespace bgl
{

	void BLogger::Log(const char* RawMessage)
	{
		std::cout << RawMessage << std::endl;
	}

	inline void BGL_LOG(const char* msg)
	{
		BLogger::Log(msg);
	}

}



