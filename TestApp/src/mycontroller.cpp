#include "mycontroller.h"
#include "core/actor/pawn/controller/controller.h"
#include "core/actor/components/movablecomponent/movablecomponent.h"
#include "core/input/input.h"

using namespace loki;

MyController::MyController()
{
	SubscribeToEvent(EVENT_ONUPDATE);
}

MyController::~MyController()
{

}

void MyController::_OnEvent( const loki::LkEvent& _Event )
{
	switch (_Event.GetEventType())
	{
	case EVENT_ONUPDATE:
		{
			if (m_Pawn)
			{
				loki::LkMovableComponent* movcomp = m_Pawn->GetComponent<loki::LkMovableComponent>();
				
				/*
				switch (GetID()%3)
				{
				case 0:
					movcomp->RotateY(0.4f);
					break;
				case 1:
					movcomp->RotateY(0.4f);
					movcomp->RotateX(0.3f);
					break;
				case 2:
					movcomp->RotateY(0.4f);
					movcomp->RotateX(0.3f);
					movcomp->RotateZ(0.5f);
				}*/

				if (g_Input->Get(KEY_ARROWUP) == KEYSTATE_DOWN)
				{
					movcomp->Translate(FORWARD);
				}
				else if (g_Input->Get(KEY_ARROWDOWN) == KEYSTATE_DOWN)
				{
					movcomp->Translate(-FORWARD);
				}

				if (g_Input->Get(KEY_ARROWLEFT) == KEYSTATE_DOWN)
				{
					movcomp->Translate(-SIDE);
				}
				else if (g_Input->Get(KEY_ARROWRIGHT) == KEYSTATE_DOWN)
				{
					movcomp->Translate(SIDE);
				}

				if (g_Input->Get(KEY_COMMA) == KEYSTATE_DOWN)
				{
					movcomp->Translate(-UP);
				}
				else if (g_Input->Get(KEY_DOT) == KEYSTATE_DOWN)
				{
					movcomp->Translate(UP);
				}
			}
			break;
		}
	}
}