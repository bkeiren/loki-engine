#pragma once

#ifndef ISTATE_H
#define ISTATE_H

//#include <SFML/Window/Event.hpp>
#include "core/statemanager.h"			// This is included so that istate.h can simply be included somewhere 
									// and statemanager will automatically be available as well.

namespace loki
{

//////////////////////////////////////////////////////////////////////////
// IState class. This class is an abstract class that defines
// the basic functionality that each state object should contain.
// This design pattern is very useful to create an engine that can handle
// any state-flow and any state-specific implementation. It is very easy
// to create two very different games since the engine no longer has to
// keep in mind the regular flow through states and logic and rendering
// details for each state. The programmer can simple define all these 
// details for each state that is required for the application, and 
// a state-manager will make sure the correct state is used.
//////////////////////////////////////////////////////////////////////////

class LkIState
{
public:
	LkIState( const char* _Name ) : m_ID(m_StateCounter), m_Name(_Name), m_Initialized(false) { ++m_StateCounter; }
	virtual ~LkIState() {}


	/*
		Update updates this state's logic.
	*/
	virtual void Update() = 0;

	/*
		Render renders this state's things to render. (What a pleasent sentence...)
	*/
	virtual void Render() = 0;

	/*
		HandleEvents handles an sf::Event. This function is called
		from outside the state class.
	*/
	//virtual void HandleEvent( sf::Event _Event ) = 0;

	/*
		Obtain the ID of the state.
	*/
	const int GetID() { return m_ID; }

	/*
		Obtain the name of the state.
	*/
	const char* GetName() { return m_Name; }
private:
	// Private default c-tor. Inheriting classes should call IState( const char* _Name ) themselves from their own c-tors.
	LkIState() : m_ID(m_StateCounter), m_Name("STATE_PRIVATE_CTOR_NAME"), m_Initialized(false) { ++m_StateCounter; }
	
	/*
		Init initializes the state.
	*/
	virtual void Init() = 0;

	/*
		ReInit is responsible for resetting a state if the state-manager
		is urged to do so. This is helpful to reset game states without
		needing to reload game assets, for example.
	*/
	virtual void ReInit() = 0;
	
	/*
		CleanUp cleans up after this state. It is responsible for cleaning up
		anything that was used/initialized by this state and should be removed.
	*/
	virtual void CleanUp() = 0;

	// This counter keeps track of the number of states that have been created and is used to assign each state a unique ID.
	static int m_StateCounter;
	
	// This boolean keeps track of whether the state has been initialized.
	bool m_Initialized;

	// The StateManager class is a friend so that it can access the private methods Init(), ReInit() and CleanUp(), and private member data.
	friend class LkStateManager;
protected:
	const int m_ID;
	const char* m_Name;
};

class DefaultState	: public LkIState
{
public:
	DefaultState()	: LkIState("DefaultState") {}

	void Init() {}
	void ReInit() {}
	void Update() {}
	void Render() {}
	//void HandleEvent( sf::Event _Event ) {}
	void CleanUp() {}
private:
};

}

#endif