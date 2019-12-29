#pragma once

#include "BTypes.h"

namespace Bagual::Utils
{
	template<typename T>
	class BBuffer : public Bagual::Types::BArray<T>
	{
		
	public:

		BBuffer(size_t length = 0)
		{
			Reserve(length);
		}

	};
}