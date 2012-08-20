#include <cassert>
#include "core/states/baseclass/istate.h"
#include "core/statemanager.h"

namespace loki
{

LkStateManager* g_StateManager = NULL;

//////////////////////////////////////////////////////////////////////////
// C-tor.
//////////////////////////////////////////////////////////////////////////
LkStateManager::LkStateManager()	:
	m_DefaultState(new DefaultState()),
	m_ActiveState(m_DefaultState)
{

}

//////////////////////////////////////////////////////////////////////////
// D-tor.
//////////////////////////////////////////////////////////////////////////
LkStateManager::~LkStateManager()
{
	// If the statemanager is destroyed, all it's states should be cleaned up and deleted.
	for (unsigned int i = 0; i < m_States.size(); ++i)
	{
		m_States[i]->CleanUp();
		delete m_States[i];
		m_States[i] = NULL;
	}

	m_ActiveState = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Registers a state with the state manager.
//////////////////////////////////////////////////////////////////////////
void LkStateManager::RegisterState( LkIState* _State )
{
	for (std::vector<LkIState*>::iterator it = m_States.begin(); it != m_States.end(); ++it)
	{
		if ( (*it)->GetID() == _State->GetID() )
		{
			LOG(VL_ERROR, "StateManager::RegisterState: Detected ID-collision between states ('%s', ID: %i)", _State->GetName(), _State->GetID());
			return;
		}
		if ( strcmp((*it)->GetName(), _State->GetName()) == 0 )	// Not cast to a bool because of a pesky performance warning.
		{
			LOG(VL_ERROR, "StateManager::RegisterState: Detected name-collision between states ('%s', ID: %i)", _State->GetName(), _State->GetID());
			return;
		}
	}

	m_States.push_back(_State);

	LOG(VL_NORMAL, "StateManager::RegisterState: Registered state '%s' ID: %i", _State->GetName(), _State->GetID());
}

//////////////////////////////////////////////////////////////////////////
// Returns the currently active state.
//////////////////////////////////////////////////////////////////////////
LkIState* LkStateManager::GetActiveState()
{
	return m_ActiveState;
}

//////////////////////////////////////////////////////////////////////////
// Calls the active state's ReInit method.
//////////////////////////////////////////////////////////////////////////
void LkStateManager::ResetActiveState()
{
	assert( m_ActiveState != NULL );

	m_ActiveState->ReInit();
}

//////////////////////////////////////////////////////////////////////////
// Sets the state with ID _ID to be the currently active state
//////////////////////////////////////////////////////////////////////////
void LkStateManager::SetActiveState( const int _ID )
{
	LkIState* state = NULL;
	for (unsigned int i = 0; i < m_States.size(); ++i)
	{
		state = m_States[i];
		if (state->GetID() == _ID)
		{
			SetActiveState(state);
			return;
		}
	}

	LOG(VL_WARN, "StateManager::SetActiveState: Could not find state with ID %i\n", _ID);
}

//////////////////////////////////////////////////////////////////////////
// Sets the state with name _Name to be the currently active state
//////////////////////////////////////////////////////////////////////////
void LkStateManager::SetActiveState( const char* _Name )
{
	LkIState* state = NULL;
	for (unsigned int i = 0; i < m_States.size(); ++i)
	{
		state = m_States[i];
		if (state->GetName() == _Name)
		{
			SetActiveState(state);
			return;
		}
	}

	LOG(VL_WARN, "StateManager::SetActiveState: Could not find state with name %s\n", _Name);
}

//////////////////////////////////////////////////////////////////////////
// Sets _State to be the active state. Implements the setting of the state
// so that the other SetActiveState methods do not need to know about it.
// This minimizes code duplication.
//////////////////////////////////////////////////////////////////////////
void LkStateManager::SetActiveState( LkIState* _State )
{
	// Just to make sure this function is never called with a NULL parameter. Should never happen, though.
	assert( _State != NULL );

	m_ActiveState = _State;

	LOG(VL_NORMAL, "StateManager::SetActiveState: Set state %i '%s'", m_ActiveState->GetID(), m_ActiveState->GetName());

	// Call the state's Init function if it has not been initialized yet.
	if (!m_ActiveState->m_Initialized)
	{
		LOG(VL_NORMAL, "StateManager::SetActiveState: Initializing state %i '%s'", m_ActiveState->GetID(), m_ActiveState->GetName());

		m_ActiveState->Init();
		m_ActiveState->m_Initialized = true;	// This can not be handled by the Init() method of the State class itself,
												// because that is a pure virtual function, so child classes have their own
												// implementations, which means that the boolean might never get set if it is
												// left to the client to set it.
	}
}

}