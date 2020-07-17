#pragma once

namespace bgl
{
	class BLogger
	{

	public:

		static void Log(const char* RawMessage);
	};

	inline void BGL_LOG(const char* msg);
}
