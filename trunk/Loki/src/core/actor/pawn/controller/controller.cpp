#include "core/actor/pawn/pawn.h"
#include "core/actor/pawn/controller/controller.h"

using namespace loki;

namespace
{
	uint32 ControllerCounter = 1;
}

LkController::LkController()	:
	m_Pawn(NULL),
	m_ID(ControllerCounter++)
{

}

LkController::~LkController()
{

}

//////////////////////////////////////////////////////////////////////////
// Possess a pawn and notifies the pawn of this.
//////////////////////////////////////////////////////////////////////////
void LkController::Possess( LkPawn* const _Pawn )
{
	// DO NOT ALTER ORDER.
	if (m_Pawn == NULL)
	{
		m_Pawn = _Pawn;
		m_Pawn->PossessedBy(this);
	}
	else
	{
		LOG(VL_WARN, "Controller::Possess: Controller still has possession of a Pawn. Call Unpossess before calling Possess");
	}
}

//////////////////////////////////////////////////////////////////////////
// Unpossess a pawn and notifies the pawn of this.
//////////////////////////////////////////////////////////////////////////
void LkController::Unpossess()
{
	// DO NOT ALTER ORDER.
	if (m_Pawn != NULL)
	{
		LkPawn* temp = m_Pawn;
		m_Pawn = NULL;
		temp->UnpossessedBy();
	}
	else
	{
		LOG(VL_NORMAL, "Controller::Unpossess: Controller does not possess a Pawn");
	}
}

//////////////////////////////////////////////////////////////////////////
// Returns the pawn that the Controller is currently in possession of.
//////////////////////////////////////////////////////////////////////////
LkPawn* LkController::GetPawn()
{
	return m_Pawn;
}

//////////////////////////////////////////////////////////////////////////
// Returns the pawn's ID.
//////////////////////////////////////////////////////////////////////////
uint32 LkController::GetID()
{
	return m_ID;
}