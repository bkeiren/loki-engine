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

void CppScript::_OnEvent( const LkEvent& _Event )
{
	switch (_Event.GetEventType())
	{
	case EVENT_ONUPDATE:
		{
			Update();
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