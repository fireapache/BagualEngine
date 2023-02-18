
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "Logger.h"

#include <iostream>

namespace bgl
{

	void BLogger::Log( const char* RawMessage )
	{
		std::cout << RawMessage << std::endl;
	}

} // namespace bgl
