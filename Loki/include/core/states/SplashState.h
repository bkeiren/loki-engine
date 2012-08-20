#pragma once

//////////////////////////////////////////////////////////////////////////
// This file can be used to define custom states, which can then be
// registered with the state manager for use in the application.
//////////////////////////////////////////////////////////////////////////

#ifndef SPLASHSTATE_H
#define SPLASHSTATE_H

#include "core/states/baseclass/istate.h"

namespace loki
{

class State_Splash	: public LkIState
{
public:
	State_Splash( const char* _Name );

	void Init();
	void ReInit();
	void Update();
	void Render();
	//void HandleEvent( sf::Event _Event );
	void CleanUp();
private:
	State_Splash();
};

}

#endif