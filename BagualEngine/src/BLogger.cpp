
#include "BLogger.h"

#include <iostream>

namespace Bagual::Utils
{

	void BLogger::Log(const char* RawMessage)
	{
		std::cout << RawMessage << std::endl;
	}

}



