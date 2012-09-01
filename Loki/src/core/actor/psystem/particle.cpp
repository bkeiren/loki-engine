#include "core/actor/psystem/particle.h"
#include "core/engine.h"

namespace loki
{

namespace
{

void DummyParticleCallback( LkParticle* _Particle )
{
	// Do nothing.
}

}

LkParticle::LkParticle()	:
	m_Age(0.0f),
	m_Lifetime(1.0f),
	m_Color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
	m_Callback(&DummyParticleCallback),
	m_Size(1.0f)
{
	memset(m_UserData, 0, 8 * sizeof(void*));
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
	m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Size = 1.0f;
	SetCallback(0);
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetOrientation(glm::quat());
}

void LkParticle::SetCallback( ParticleCallback _Callback )
{
	m_Callback = (_Callback == 0)?(&DummyParticleCallback):(_Callback);
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

	// Call the callback. We don't have to check for a NULL callback because the callback will always be either valid or 
	// pointing to the DummyParticleCallback function.
	m_Callback(this);
}

}