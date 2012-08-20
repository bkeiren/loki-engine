#include "core/engine.h"
#include "core/states/Splashstate.h"

using namespace loki;

State_Splash::State_Splash( const char* _Name )	:
	LkIState(_Name)
{

}

void State_Splash::Init()
{

}

void State_Splash::ReInit()
{

}

void State_Splash::Update()
{
	g_StateManager->SetActiveState("State_Test");
}

void State_Splash::Render()
{

}

/*
void State_Splash::HandleEvent( sf::Event _Event )
{

}
*/

void State_Splash::CleanUp()
{

}