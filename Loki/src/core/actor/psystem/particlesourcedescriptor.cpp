#include "core/actor/psystem/particlesourcedescriptor.h"

namespace loki
{

#define PARTICLESOURCE_DEFAULT_QUOTA	32

LkParticleSourceDescriptor::LkParticleSourceDescriptor()	:
	m_SpawnRate(1.0f),
	m_Lifetime(1.0f),
	m_Birth(0.0f),
	m_Quota(PARTICLESOURCE_DEFAULT_QUOTA),
	m_SpawnQuota(1),
	m_Callback(0),
	m_Position(glm::vec3(0.0f, 0.0f, 0.0f))
{
	memset(m_DefaultUserData, 0, 8 * sizeof(void*));
}

LkParticleSourceDescriptor::~LkParticleSourceDescriptor()
{

}

#ifdef PARTICLESOURCE_DEFAULT_QUOTA
#undef PARTICLESOURCE_DEFAULT_QUOTA
#endif

}