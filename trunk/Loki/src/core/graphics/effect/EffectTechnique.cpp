#include "core/graphics/effect/EffectTechnique.h"

namespace loki
{

namespace graphics
{

EffectTechnique::EffectTechnique( void* _Technique, const std::string& _TechniqueName )	:
	m_Name(_TechniqueName)
	,m_CGTechnique(_Technique)
{

}

EffectTechnique::EffectTechnique()
{
	ILLEGAL_CTOR_ERROR("EffectTechnique")
}

EffectTechnique::~EffectTechnique()
{

}

const std::string& EffectTechnique::GetName() const
{
	return m_Name;
}

}

}
