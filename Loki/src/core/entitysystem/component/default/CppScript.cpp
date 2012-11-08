#include "core/entitysystem/component/default/CppScript.h"

namespace loki
{

namespace components
{

CppScript::CppScript()
{
	SubscribeToEvent(EVENT_ONUPDATE);
}

CppScript::~CppScript()
{
	UnsubscribeFromEvent(EVENT_ONUPDATE);
}

void CppScript::_HandleEvent( const LkEvent& _Event )
{
	switch (_Event.GetEventType())
	{
	case EVENT_ONUPDATE:
		{
			Update();
			break;
		}
	case EVENT_COMPONENT_ENABLED:	// Not officially registered to this, but 
									// is dispatched privately by the Component class.
		{
			Enabled();
			break;
		}
	case EVENT_COMPONENT_DISABLED:	// Not officially registered to this, but 
									// is dispatched privately by the Component class.
		{
			Disabled();
			break;
		}

	}
}

void CppScript::_Init()
{
	Awake();
}

void CppScript::_Terminate()
{
	Stop();
}

}

}