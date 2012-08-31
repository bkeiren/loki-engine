#include "core/actor/psystem/particlesource.h"
#include "core/actor/psystem/particle.h"
#include "core/engine.h"

namespace loki
{

LkParticleSource::LkParticleSource( int _Quota /* = 1 */ )	:
	m_SpawnRate(1.0f),
	m_Age(0.0f),
	m_Lifetime(0.0f),
	m_Birth(0.0f),
	m_AgeSinceLastSpawn(0.0f),
	m_SpawnQuota(1)
{
	SetQuota(_Quota);
}

LkParticleSource::~LkParticleSource()
{

}

void LkParticleSource::SetQuota( int _Quota )
{
	int PreviousQuota = GetQuota();

	// If _Quota is greater than the current quota, more storage space is allocated.
	// If _Quota is smaller than the current quota, the storage space that 
	// is no longer needed is deleted (by calling each element's destructor).
	m_Particles.resize(_Quota, 0);

	int CurrentQuota = GetQuota();

	if (CurrentQuota > PreviousQuota)
	{
		for (int i = PreviousQuota - 1; i < CurrentQuota; ++i)
		{
			m_Particles[i] = new LkParticle();
		}
	}
}

int LkParticleSource::GetQuota() const
{
	return m_Particles.max_size();
}

void LkParticleSource::_Update()
{
	m_Age += g_Engine->GetFrameTime();

	if (m_Age < m_Birth)
	{
		return;
	}

	m_AgeSinceLastSpawn += g_Engine->GetFrameTime();

	if (m_AgeSinceLastSpawn >= (1.0f / m_SpawnRate))
	{
		_Spawn(m_SpawnQuota);
		m_AgeSinceLastSpawn = 0.0f;
	}

	for (ParticlesIter it = m_Particles.begin(); it != m_Particles.end(); ++it)
	{
		(*it)->_Update();
	}
}

void LkParticleSource::_Spawn( int _Quota )
{
	for (ParticlesIter it = m_Particles.begin(); it != m_Particles.end(); ++it)
	{
		if (!((*it)->IsAlive()))
		{
			(*it)->Resurrect();

			--_Quota;

			if (_Quota <= 0)
			{
				return;
			}
		}
	}
}

}