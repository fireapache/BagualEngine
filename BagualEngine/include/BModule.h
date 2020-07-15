#pragma once


namespace bgl
{
	class BIModule
	{

	public:

		virtual void Init() = 0;
		virtual void Tick() = 0;
		virtual void Term() = 0;

	};

	class BModuleBase : public BIModule
	{
		
	public:

		

	};
}