#include "core/actor/psystem/particlesystem.h"
#include "core/actor/psystem/particlesource.h"
#include "core/actor/psystem/particlesystemdescriptor.h"
#include "core/renderer/effect/effectmanager.h"

namespace loki
{

renderer::LkEffect* LkParticleSystem::m_CGEffect = 0;

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

	if (!m_CGEffect)
	{
		m_CGEffect = renderer::g_EffectManager->CreateEffectFromFile("resources//shaders//particles.cgfx", "CG_Effect_Particles");
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

void LkParticleSystem::Render( const glm::mat4& _ViewMatrix, const glm::mat4& _ProjectionMatrix )
{
	glm::mat4 m = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							m_Descriptor.m_Position.x, m_Descriptor.m_Position.y, m_Descriptor.m_Position.z, 1.0f);

	for (ParticleSourcesConstIter it = m_Sources.begin(); it != m_Sources.end(); ++it)
	{
		LkParticleSource* source = (*it);

		glm::mat4 m2 = glm::gtc::matrix_transform::translate(m, source->GetPosition());
		
		for (LkParticleSource::ParticlesConstIter it2 = source->m_Particles.begin(); it2 != source->m_Particles.end(); ++it2)
		{
			LkParticle* p = (*it2);
			
			glm::mat4 m3 = m2 * p->GetTransformation();

			renderer::LkEffectParameter* param = m_CGEffect->GetParameterBySemantic("LKMODELMATRIX");
			if (param)
			{
				param->Set(m3);
			}

			param = m_CGEffect->GetParameterBySemantic("LKVIEWMATRIX");
			if (param)
			{
				param->Set(_ViewMatrix);
			}

			param = m_CGEffect->GetParameterBySemantic("LKMODELSCALE");
			if (param)
			{
				param->Set(p->m_Size);
			}

			param = m_CGEffect->GetParameterBySemantic("LKMODELVIEWPROJ");
			if (param)
			{
				param->Set(_ProjectionMatrix * _ViewMatrix * m3);
			}

			while (m_CGEffect->HasNextPass())
			{
				glBegin(GL_QUADS);
				{
					glColor4f(p->m_Color.r, p->m_Color.g, p->m_Color.b, p->m_Color.a);

					glTexCoord2f(0.0f, 0.0f);
					glVertex3f(-0.5f, 0.5f, 0.0f);
					
					glTexCoord2f(0.0f, 1.0f);
					glVertex3f(-0.5f, -0.5f, 0.0f);

					glTexCoord2f(1.0f, 1.0f);
					glVertex3f(0.5f, -0.5f, 0.0f);

					glTexCoord2f(1.0f, 0.0f);
					glVertex3f(0.5f, 0.5f, 0.0f);
				}
				glEnd();
			}
		}
	}
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