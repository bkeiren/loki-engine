#pragma once

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "core/eventsystem/eventlistener/eventlistener.h"
#include "core/actor/pawn/pawn.h"

namespace loki
{

class LkPawn;

/*
	Base class for all Controllers that can control Pawns. The Controller class keeps track of certain
	state information of it's pawn (For example, state of cheats such as god mode, whether the Pawn can
	use certain items, etc.) This is because the Controller is the basic class that is accessed by
	players and bots in order to handle the way its Pawn acts. The Pawn class basically only serves
	as the class that contains visual and physical representations, etc.
*/
class LkController	: public LkEventListener
{
public:
	LkController();
	virtual ~LkController() = 0;

	void Possess( LkPawn* const _Pawn );
	void Unpossess();

	LkPawn* GetPawn();

	uint32 GetID();
protected:
	virtual void _OnEvent( const LkEvent& _Event ) = 0;

	LkPawn* m_Pawn;
	uint32 m_ID;
};

}

#endif