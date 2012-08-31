#include "core/actor/psystem/particle.h"
#include "core/engine.h"

namespace loki
{

LkParticle::LkParticle()	:
	m_Age(0.0f),
	m_Lifetime(1.0f)
{

}

LkParticle::~LkParticle()
{

}

bool LkParticle::IsAlive() const
{
	return m_IsAlive;
}

void LkParticle::Kill()
{
	m_IsAlive = false;
}

void LkParticle::Resurrect()
{
	Reset();
	m_IsAlive = true;
}

void LkParticle::Reset()
{
	m_IsAlive = false;
	m_Age = 0.0f;
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetOrientation(glm::quat());
}

void LkParticle::_Update()
{
	if (!m_IsAlive)
	{
		return;
	}

	m_Age += g_Engine->GetFrameTime();
	if (m_Age > m_Lifetime)
	{
		Kill();
		return;
	}
}

}