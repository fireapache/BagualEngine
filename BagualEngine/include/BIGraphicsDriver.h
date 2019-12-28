#pragma once

namespace Bagual
{
	namespace GraphicsDriver
	{
		class BIGraphicsDriver
		{

		public:

			virtual void Render() = 0;
			virtual void Delay(unsigned int ms) = 0;

		};
	}
}