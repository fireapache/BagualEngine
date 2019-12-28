
#include "BLogger.h"

#include <iostream>

namespace Bagual::Log
{

	void BLogger::Log(const char* RawMessage)
	{
		std::cout << RawMessage << std::endl;
	}

}



