#pragma once


namespace bgl
{
	class BIModule
	{

	public:

		virtual void Init() = 0;
		virtual void BeginPlay() = 0;
		virtual void EndPlay() = 0;
		virtual void Term() = 0;
		virtual void Tick() = 0;

	};

	class BModuleBase : public BIModule
	{
		
	public:

		virtual void Init() { }
		virtual void BeginPlay() { };
		virtual void EndPlay() { };
		virtual void Term() { }
		virtual void Tick() { }

	};
}