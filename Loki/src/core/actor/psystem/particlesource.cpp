#include "core/actor/psystem/particlesource.h"
#include "core/actor/psystem/particle.h"
#include "core/engine.h"

namespace loki
{

LkParticleSource::LkParticleSource( const LkParticleSourceDescriptor& _Descriptor )	:
	m_Descriptor(_Descriptor),
	m_Age(0.0f),
	m_AgeSinceLastSpawn(0.0f)
{
	SetQuota(m_Descriptor.m_Quota);
}

LkParticleSource::~LkParticleSource()
{

}

void LkParticleSource::SetQuota( int _Quota )
{
	if (_Quota < 0)
	{
		return;
	}

	int PreviousQuota = GetQuota();

	// If _Quota is greater than the current quota, more storage space is allocated.
	// If _Quota is smaller than the current quota, the storage space that 
	// is no longer needed is deleted (by calling each element's destructor).
	m_Particles.resize(_Quota, 0);

	int CurrentQuota = GetQuota();

	if (CurrentQuota > PreviousQuota)
	{
		for (int i = PreviousQuota; i < CurrentQuota; ++i)
		{
			m_Particles[i] = new LkParticle();
		}
	}
}

int LkParticleSource::GetQuota() const
{
	return m_Particles.capacity();
}

void LkParticleSource::_Update()
{
	m_Age += g_Engine->GetFrameTime();

	if (m_Age < m_Descriptor.m_Birth)
	{
		return;
	}

	m_AgeSinceLastSpawn += g_Engine->GetFrameTime();

	if (m_AgeSinceLastSpawn >= (1.0f / m_Descriptor.m_SpawnRate))
	{
		_Spawn();
		m_AgeSinceLastSpawn = 0.0f;
	}

	for (ParticlesIter it = m_Particles.begin(); it != m_Particles.end(); ++it)
	{
		(*it)->_Update();
	}
}

void LkParticleSource::_Spawn()
{
	int sq = m_Descriptor.m_SpawnQuota;

	for (ParticlesIter it = m_Particles.begin(); it != m_Particles.end(); ++it)
	{
		if (!((*it)->IsAlive()))
		{
			(*it)->Resurrect();
			(*it)->SetCallback(m_Descriptor.m_Callback);
			memcpy_s((*it)->m_UserData, 8 * sizeof(void*), m_Descriptor.m_DefaultUserData, 8 * sizeof(void*));

			--sq;

			if (sq <= 0)
			{
				return;
			}
		}
	}
}

}
