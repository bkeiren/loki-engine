#include "core/renderer/effect/effect.h"
#include "core/renderer/effect/effectparameter.h"

#include <iostream>

#include "Cg/cgGL.h"

namespace loki
{

namespace renderer
{

LkEffect::LkEffect( void* _Effect, const std::string& _EffectName )	:
	m_Name(_EffectName),
	m_CGEffect(_Effect),
	m_CGTechnique(0),
	m_CGCurrentPass(0),
	m_HasValidTechnique(false)
{
	m_CGTechnique = cgGetFirstTechnique((CGeffect)m_CGEffect);
	while (m_CGTechnique != NULL && cgValidateTechnique((CGtechnique)m_CGTechnique) == CG_FALSE)
	{
		LOG(VL_ERROR, "Cg: Technique '%s' did not pass validation (Effect: %s)", cgGetTechniqueName((CGtechnique)m_CGTechnique), m_Name.c_str());
		m_CGTechnique = cgGetNextTechnique((CGtechnique)m_CGTechnique);
	}

	if (m_CGTechnique == NULL || cgIsTechniqueValidated((CGtechnique)m_CGTechnique) == CG_FALSE)
	{
		LOG(VL_ERROR, "Cg: Could not find any valid techniques for effect %s", m_Name.c_str());
		m_HasValidTechnique = false;
	}
	else
	{
		m_HasValidTechnique = true;
	}

	_LoadNamedParameters();

	LOG(VL_NORMAL, "Effect::Effect: Created effect '%s'", _EffectName.c_str());
}

LkEffect::LkEffect()
{
	ILLEGAL_CTOR_ERROR("Effect");
}


LkEffect::~LkEffect()
{
	if (m_CGEffect != NULL)
	{
		cgDestroyEffect((CGeffect)m_CGEffect);
		m_CGEffect = NULL;
	}
}

const std::string& LkEffect::GetName() const
{
	return m_Name;
}

LkEffectParameter* LkEffect::GetParameter( const std::string& _ParameterName )
{
	ParametersConstItr it = m_Parameters.find(_ParameterName);
	if (it != m_Parameters.end())
	{
		return (*it).second;
	}
	LOG(VL_ERROR, "Effect::GetParameter: Parameter '%s' does not exist", _ParameterName);
	return 0;
}

LkEffectParameter* LkEffect::GetParameterBySemantic( const std::string& _Semantic )
{
	// First attempt to find the semantic parameter in the 'cache' map.
	ParametersConstItr it = m_ParametersBySemantic.find(_Semantic);
	if (it != m_ParametersBySemantic.end())
	{
		return (*it).second;
	}

	// No existing semantic parameter found, try searching for it because it may have just
	// not been cached yet.
	CGparameter param = cgGetEffectParameterBySemantic((CGeffect)m_CGEffect, _Semantic.c_str());
	if (param != 0)
	{
		LkEffectParameter* effectparameter = new LkEffectParameter(param, _Semantic);
		m_Parameters.insert(ParametersPair(_Semantic, effectparameter));
		return effectparameter;
	}

	LOG(VL_ERROR, "Effect::GetParameterBySemantic: Parameter with semantic '%s' does not exists", _Semantic);
	return 0;
}

void LkEffect::_LoadNamedParameters()
{
	CGparameter param = cgGetFirstEffectParameter((CGeffect)m_CGEffect);
	while (param)
	{
		std::string paramname = std::string(cgGetParameterName(param));
		m_Parameters.insert(ParametersPair(paramname, new LkEffectParameter(param, paramname)));

		param = cgGetNextParameter(param);
	}
}

bool LkEffect::HasNextPass()
{
	if (m_CGCurrentPass == 0)
	{
		m_CGCurrentPass = cgGetFirstPass((CGtechnique)m_CGTechnique);

		// TODO: Ask effect manager to update shared variables or global semantics?
	}
	else
	{
		cgResetPassState((CGpass)m_CGCurrentPass);
		m_CGCurrentPass = cgGetNextPass((CGpass)m_CGCurrentPass);
	}

	if (m_CGCurrentPass == 0)
	{
		return false;
	}
	else
	{
		cgSetPassState((CGpass)m_CGCurrentPass);
	}

	return true;
}

}

}