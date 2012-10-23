#include "util/hash/hash_fnv/hash_fnv.h"
#include <sstream>
#include "core/actor/actor.h"
#include "core/actor/handle/handle.h"

namespace loki
{

LkActor::LkActor( const char* _Name, game::LkLevel* _Level )	:
	m_Level(_Level)
{
	// Hash the name and check its validity by checking all other objects to see whether an
	// object with the same ID/name already exists. If it does, change the name by appending a random value
	// to the name and re-hashing it.
	m_ID = GetHashForName(_Name);
	m_Name = std::string(_Name);

	// TODO: Check all objects that m_Level possesses to find out whether there exists an Object that with the same name.

	if (!m_Level)
	{
		LOG(VL_ERROR, "Actor '%s' was instantiated with NULL level", m_Name.c_str());
	}
}

LkActor::LkActor()
{
	// Just to be safe: Log that an actor was created in an illegal way (Even though this c-tor should not be able
	// to be accessed from anywhere.
	//LOG(VL_ERROR, "class Actor was instantiated with illegal c-tor");
	ILLEGAL_CTOR_ERROR("Actor");
}

LkActor::~LkActor()
{
	_ClearComponents();
}

uint32 LkActor::GetHashForName( const char* _Name )
{
	return util::Hash_FNV32(_Name);
}

const std::string& LkActor::GetName() const
{
	return m_Name;
}

const uint32 LkActor::GetID() const
{
	return m_ID;
}

void LkActor::_ClearComponents()
{
	for (ComponentMap::iterator it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		delete (*it).second;
	}
	m_Components.clear();
}

void LkActor::_Touch( LkActor* _Actor )
{

}

void LkActor::_Untouch( LkActor* _Actor )
{

}

void LkActor::_OnEvent( const LkEvent& _Event )
{
	switch (const_cast<LkEvent&>(_Event).GetEventType())
	{
	case EVENT_BASE:	// Just to indicate how events should be used...
		{
			break;
		}
	}
}

std::string LkActor::ToString() const
{
	std::stringstream str;
	str << "Actor::" <<	\
		   "\nName:\t\t" << m_Name << 
		   "\nID:\t\t" << m_ID;

	return str.str();
}

}