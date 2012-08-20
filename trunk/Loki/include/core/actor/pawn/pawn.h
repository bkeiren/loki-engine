#pragma once

#ifndef PAWN_H
#define PAWN_H

#include "core/actor/actor.h"

namespace loki
{

class LkController;

/*
	Base class for all controllable Actors.
*/
class LkPawn	: public LkActor
{
	friend class game::LkLevel;
public:
	void PossessedBy( LkController* const _Controller );		// Called by the Pawn's (new) controller when it possess the Pawn.
	void UnpossessedBy();	// Called by the Pawn's controller when it releases possession of the Pawn.

	virtual std::string ToString();
private:
	LkPawn( const char* _Name, game::LkLevel* _Level );
	LkPawn();	// Private default c-tor.
	virtual ~LkPawn();

protected:
	LkController* m_Controller;
};

}

#endif