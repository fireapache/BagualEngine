
// clang-format off
#include "Bagual.pch.h"
// clang-format on

#include "Module.h"
#include "Scene.h"

namespace bgl
{
	void BModule::setSceneDirty()
	{
		for( const auto& comp : m_components )
		{
			const auto meshComp = dynamic_cast< BMeshComponent* >( comp );

			if( !meshComp )
			{
				continue;
			}

			if( meshComp->m_scene )
			{
				meshComp->setSceneDirty();
				break;
			}
		}
	}
}
