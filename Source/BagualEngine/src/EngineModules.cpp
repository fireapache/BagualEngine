
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "EngineModules.h"

#include "BagualEngine.h"
#include "CameraManager.h"
#include "GraphicsPlatform.h"
#include "PlatformBase.h"
#include "Scene.h"
#include "Viewport.h"
#include <Draw.h>
#include <imgui.h>

namespace bgl
{
	void BModuleManager::init()
	{
		guiTickFunc = [ this ]()
		{
			IM_ASSERT( ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!" );

			ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(
				ImVec2( main_viewport->GetWorkPos().x, main_viewport->GetWorkPos().y ),
				ImGuiCond_FirstUseEver );
			ImGui::SetNextWindowSize( ImVec2( 300, 400 ), ImGuiCond_FirstUseEver );

			ImGuiWindowFlags window_flags = 0;
			if( !ImGui::Begin( "Module Manager", nullptr, window_flags ) )
			{
				ImGui::End();
				return;
			}

			ImGui::PushItemWidth( ImGui::GetFontSize() * -12 );

			auto& modules = BEngine::Instance().getModules();

			std::string imguiId{ "" };

			for( const auto& module : modules )
			{
				if( module->showOnModuleManager() == false )
				{
					continue;
				}

				const char* moduleName = module->getName();
				const bool bInitialized = module->initialized();

				ImGui::Text( moduleName );
				ImGui::SameLine();

				if( bInitialized )
				{
					bool& bhidden = module->isHidden_mutable();
					imguiId = "Hidden##";
					imguiId.append( module->getName() );
					ImGui::Checkbox( imguiId.c_str(), &bhidden );
				}
				else
				{
					if( module->pendingTasks.bInitialize )
					{
						ImGui::TextColored( ImVec4( 1.f, 0.1f, 0.1f, 1.f ), "Initializing..." );
					}
					else
					{
						imguiId = "Initialize##";
						imguiId.append( module->getName() );

						if( ImGui::Button( imguiId.c_str() ) )
						{
							module->pendingTasks.bInitialize = true;
						}
					}
				}
			}

			ImGui::End();
		};

		BEngine::Instance().registerGuiTickFunc( &guiTickFunc );
	}

	void BModuleManager::tick()
	{
	}

	void BModuleManager::destroy()
	{
	}
} // namespace bgl