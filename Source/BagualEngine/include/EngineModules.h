#pragma once

#include "Common.h"
#include "Module.h"

namespace bgl
{
	class BModuleManager : public BModule
	{
	public:
		BModuleManager()
			: BModule( "Module Manager" ){};

		GuiTickFuncType guiTickFunc;

		void init() override;
		void tick() override;
		void destroy() override;

		bool autoInit() const override
		{
			return true;
		}

		bool autoShow() const override
		{
			return true;
		}

		bool showOnModuleManager() const override
		{
			return false;
		}
		
	};

} // namespace bgl