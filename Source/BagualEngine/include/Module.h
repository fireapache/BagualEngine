#pragma once

#include "Types.h"

namespace bgl
{
	class BNode;
	class BComponent;
} // namespace bgl

namespace bgl
{
	class BModule
	{
	protected:
		std::string m_name;
		BArray< BNode* > m_nodes;
		BArray< BComponent* > m_components;
		bool m_hidden{ false };

	public:
		BModule( const char* name = nullptr )
		{
			if( name )
			{
				m_name = name;
			}
		}

		BModule( const BModule& mod )
		{
			m_name = mod.getName();
		}
		
		virtual ~BModule() = default;

		virtual void init(){};

		virtual void show()
		{
			m_hidden = false;
		}

		virtual void hide()
		{
			m_hidden = true;
		}

		virtual void destroy(){};

		void term()
		{
			if( initialized() )
			{
				destroy();
			}
		}

		struct PendingTasks
		{
			bool bInitialize{ false };
		};

		PendingTasks pendingTasks;

		virtual void tick(){};

		[[nodiscard]] const char* getName() const
		{
			return m_name.c_str();
		}

		[[nodiscard]] virtual bool initialized() const
		{
			return false;
		}

		[[nodiscard]] virtual bool autoInit() const
		{
			return false;
		}

		[[nodiscard]] virtual bool autoShow() const
		{
			return false;
		}

		[[nodiscard]] bool isHidden() const
		{
			return m_hidden;
		}

		[[nodiscard]] bool& isHidden_mutable()
		{
			return m_hidden;
		}

		[[nodiscard]] virtual bool showOnModuleManager() const
		{
			return true;
		}

		[[nodiscard]] BArray< BNode* >& getNodes()
		{
			return m_nodes;
		}

		[[nodiscard]] BArray< BComponent* > getComponents()
		{
			return m_components;
		}
	};
} // namespace bgl