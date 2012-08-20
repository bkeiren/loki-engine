#pragma once

//////////////////////////////////////////////////////////////////////////
// This file can be used to define custom states, which can then be
// registered with the state manager for use in the application.
//////////////////////////////////////////////////////////////////////////

#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "core/states/baseclass/istate.h"

namespace loki
{

namespace renderer
{
class SphereModel;
}

class State_Test	: public LkIState
{
public:
	State_Test( const char* _Name );

	void Init();
	void ReInit();
	void Update();
	void Render();
	//void HandleEvent( sf::Event _Event );
	void CleanUp();
private:
	State_Test();

	//////////////////////////////////////////////////////////////////////////
	// Data
	renderer::SphereModel* m_SphereModel;
};

}

#endif