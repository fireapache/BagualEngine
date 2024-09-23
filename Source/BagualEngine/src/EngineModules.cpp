
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
#include <chrono>

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

			// FPS display

			const auto lastFrameTime = BEngine::GraphicsPlatform().getGraphicsDriver()->FrameTime();
			const std::chrono::duration< double, std::milli > timeDelta = lastFrameTime;

			if( timeDelta.count() < 0.01 )
			{
				ImGui::TextUnformatted( "Frame Time: 0.00ms" );
				ImGui::TextUnformatted( "FPS: 0" );
			}
			else
			{
				ImGui::Text( "Frame Time: %.2fms", timeDelta );
				ImGui::Text( "FPS: %.f", 1000.0 / timeDelta.count() );
			}
			

			// module list

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
				ImGui::SetCursorPosX( 160 );

				if( bInitialized )
				{
					if( module->isHidden() )
					{
						imguiId = "Show##";
						imguiId.append( module->getName() );

						if( ImGui::Button( imguiId.c_str() ) )
						{
							module->show();
						}
					}
					else
					{
						imguiId = "Hide##";
						imguiId.append( module->getName() );

						if( ImGui::Button( imguiId.c_str() ) )
						{
							module->hide();
						}
					}

					ImGui::SameLine();

					ImGui::PushStyleColor( ImGuiCol_Button, ImColor( 120, 0, 0 ).Value );
					ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImColor( 150, 0, 0 ).Value );
					ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImColor( 180, 0, 0 ).Value );
					imguiId = "Terminate##";
					imguiId.append( module->getName() );
					const bool bTerminatePressed = ImGui::Button( imguiId.c_str() );
					ImGui::PopStyleColor( 3 );

					if( bTerminatePressed )
					{
						module->pendingTasks.bTerminate = true;
					}
				}
				else
				{
					if( module->pendingTasks.bInitialize )
					{
						ImGui::TextColored( ImColor( 0, 180, 0 ).Value, "Initializing..." );
					}
					else
					{
						ImGui::PushStyleColor( ImGuiCol_Button, ImColor( 0, 120, 0 ).Value );
						ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImColor( 0, 150, 0 ).Value );
						ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImColor( 0, 180, 0 ).Value );
						imguiId = "Initialize##";
						imguiId.append( module->getName() );
						const bool bInitializePressed = ImGui::Button( imguiId.c_str() );
						ImGui::PopStyleColor( 3 );

						if( bInitializePressed )
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