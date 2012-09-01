#include "core/actor/psystem/particlesystemdescriptor.h"

namespace loki
{

LkParticleSystemDescriptor::LkParticleSystemDescriptor()	:
	m_Position(glm::vec3(0.0f, 0.0f, 0.0f))
{

}

LkParticleSystemDescriptor::~LkParticleSystemDescriptor()
{

}

LkParticleSourceDescriptor& LkParticleSystemDescriptor::AddSource()
{
	m_SourceDescriptors.push_back(LkParticleSourceDescriptor());
	return m_SourceDescriptors.back();
}

}