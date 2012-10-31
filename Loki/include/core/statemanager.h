#pragma once

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
//#include "core/states/baseclass/istate.h"

namespace loki
{

class LkIState;

class LkStateManager
{
	friend class LokiEngine;
public:
	/*
		Registers a new state with the state-manager.
		Also checks the state's name and ID for any collisions with already registered state and notifies
		the user if any such collision is detected.
	*/
	void RegisterState( LkIState* _State );

	/*
		Returns the currently active state.
	*/
	LkIState* GetActiveState();

	/*
		Calls the active state's ReInit method.
	*/
	void ResetActiveState();

	/*
		Sets the state with ID _ID to be the active state.
	*/
	void SetActiveState( const int32 _ID );

	/*
		Sets the state with name _Name to be the active state.
	*/
	void SetActiveState( const char* _Name );
private:
	LkStateManager();
	~LkStateManager();

	/*
		Sets the state that is passed to be the active state. This is a private function that
		should not (and therefore can not) be used outside of the class. It is used
		to minimize duplicate code by letting this function implement the actual setting of
		the active state and letting the public SetActiveState methods implement the actual
		finding of the correct state.
	*/
	void SetActiveState( LkIState* _State );

	LkIState* m_ActiveState;
	LkIState* m_DefaultState;
	std::vector<LkIState*> m_States;
};

extern LkStateManager* g_StateManager;

}

#endif