#include "core/actor/psystem/particlesystem.h"
#include "core/actor/psystem/particlesource.h"

namespace loki
{

volatile unsigned int LkParticleSystem::m_SystemCounter = 0;

LkParticleSystem::LkParticleSystem( LkParticleSystemDescriptor& _Descriptor, game::LkLevel* _Level )	:
	LkActor((std::string("ParticleSystem") + util::LexicalCast(m_SystemCounter++)).c_str(), _Level),
	m_Descriptor(_Descriptor)
{
	SubscribeToEvent(EVENT_PREUPDATE);

	// TODO: Initiate data from descriptor.
}

LkParticleSystem::LkParticleSystem()
{
	ILLEGAL_CTOR_ERROR("ParticleSystem");
}

LkParticleSystem::~LkParticleSystem()
{
	for (ParticleSourcesIter it = m_Sources.begin(); it != m_Sources.end(); ++it)
	{
		delete (*it);
	}
	m_Sources.clear();
}

const LkParticleSystemDescriptor& LkParticleSystem::GetDescriptor() const
{
	return m_Descriptor;
}

void LkParticleSystem::_OnEvent( const LkEvent& _Event )
{
	switch (_Event.GetEventType())
	{
	case EVENT_PREUPDATE:
		{
			for (ParticleSourcesIter it = m_Sources.begin(); it != m_Sources.end(); ++it)
			{
				(*it)->_Update();
			}
			break;
		}
	}
}

}