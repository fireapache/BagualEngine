#pragma once

namespace bgl
{
	class BLogger
	{

	public:

		static void Log(const char* RawMessage);
	};
}

#ifndef BGL_LOG
#define BGL_LOG(msg) Bagual::BLogger::Log(msg)
#endif // !BGL_LOG
