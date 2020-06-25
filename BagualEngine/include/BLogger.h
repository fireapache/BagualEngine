#pragma once

namespace Bagual::Utils
{
	class BLogger
	{

	public:

		static void Log(const char* RawMessage);
	};
}

#ifndef BGL_LOG
#define BGL_LOG(msg) Bagual::Utils::BLogger::Log(msg)
#endif // !BGL_LOG
