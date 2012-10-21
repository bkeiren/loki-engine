#include <sstream>
#include "core/actor/actor.h"
#include "core/actor/pawn/pawn.h"
#include "core/actor/pawn/controller/controller.h"
#include "core/actor/components/movablecomponent/movablecomponent.h"
#include "core/actor/components/rendercomponent/rendercomponent.h"

using namespace loki;

loki::LkPawn::LkPawn( const char* _Name, game::LkLevel* _Level )	:
	LkActor(_Name, _Level),
	m_Controller(NULL)
{
	AddComponent<LkMovableComponent>();
	AddComponent<LkRenderComponent>();
	//AddComponent<LkPhysicsComponent>();
}

LkPawn::LkPawn()
{

}

LkPawn::~LkPawn()
{

}

//////////////////////////////////////////////////////////////////////////
// Gets called by a pawn's controller when it takes possession of the pawn.
//////////////////////////////////////////////////////////////////////////
void LkPawn::PossessedBy( LkController* const _Controller )
{
	if (_Controller->GetPawn() == this)
	{
		m_Controller = _Controller;
	}
	else
	{
		LOG(VL_WARN, "Pawn::PossessedBy: Corrupted call. Controller has not actually taken possession");
	}
}

//////////////////////////////////////////////////////////////////////////
// Gets called by a pawns controller when it releases possession of the pawn.
//////////////////////////////////////////////////////////////////////////
void LkPawn::UnpossessedBy()
{
	if (m_Controller->GetPawn() == NULL)
	{
		m_Controller = NULL;
	}
	else
	{
		LOG(VL_WARN, "Pawn::UnpossessedBy: Corrupted call. Controller has not actually released possession");
	}
}

std::string LkPawn::ToString()
{
	std::stringstream str;
	str << LkActor::ToString();
	str << "\nController:\t";
	if (m_Controller)
	{
		str << m_Controller->GetID();
	}
	else
	{
		str << "<NULL>";
	}
	
	return str.str();
}