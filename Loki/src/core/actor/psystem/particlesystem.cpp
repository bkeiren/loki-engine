#include "core/actor/psystem/particlesystem.h"
#include "core/actor/psystem/particlesource.h"
#include "core/actor/psystem/particlesystemdescriptor.h"

namespace loki
{

LkParticleSystem::LkParticleSystem( LkParticleSystemDescriptor& _Descriptor, const char* _Name, game::LkLevel* _Level )	:
	LkActor(_Name, _Level),
	m_Descriptor(_Descriptor)
{
	SubscribeToEvent(EVENT_PREUPDATE);

	// TODO: Initiate data from descriptor.

	if (m_Descriptor.m_SourceDescriptors.size() <= 0)
	{
		LOG(VL_WARN, "ParticleSystem::ParticleSystem: System descriptor has no source descriptors");
	}

	for (LkParticleSystemDescriptor::ParticleSourcesConstIter it = m_Descriptor.m_SourceDescriptors.begin(); it != m_Descriptor.m_SourceDescriptors.end(); ++it)
	{
		m_Sources.push_back(new LkParticleSource((*it)));
	}
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

const glm::vec3& LkParticleSystem::GetPosition() const
{
	return m_Descriptor.m_Position;
}

void LkParticleSystem::SetPosition( const glm::vec3& _Position )
{
	m_Descriptor.m_Position = _Position;
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