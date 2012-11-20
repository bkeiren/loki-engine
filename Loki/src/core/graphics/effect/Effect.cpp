#include "core/graphics/effect/Effect.h"
#include "core/graphics/effect/EffectParameter.h"
#include "core/graphics/effect/EffectTechnique.h"

#include <iostream>

#include "Cg/cgGL.h"

namespace loki
{

namespace graphics
{

Effect::Effect( void* _Effect, const std::string& _EffectName )	:
	m_Name(_EffectName),
	m_CGEffect(_Effect),
	m_CGTechnique(0),
	m_CGCurrentPass(0),
	m_HasValidTechnique(false)
{
	_LoadTechniques();
	_LoadNamedParameters();

	LOG(VL_NORMAL, "Effect::Effect: Created effect '%s'", _EffectName.c_str());
}

Effect::Effect()
{
	ILLEGAL_CTOR_ERROR("Effect");
}


Effect::~Effect()
{
	for (ParametersIter it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
	{
		delete (*it).second;
	}
	m_Parameters.clear();

	for (TechniquesIter it = m_Techniques.begin(); it != m_Techniques.end(); ++it)
	{
		delete (*it).second;
	}
	m_Techniques.clear();

	if (m_CGEffect != NULL)
	{
		cgDestroyEffect((CGeffect)m_CGEffect);
		m_CGEffect = NULL;
	}
}

const std::string& Effect::GetName() const
{
	return m_Name;
}

EffectParameter* Effect::GetParameter( const std::string& _ParameterName )
{
	ParametersConstIter it = m_Parameters.find(_ParameterName);
	if (it != m_Parameters.end())
	{
		return (*it).second;
	}
	LOG(VL_ERROR, "Effect::GetParameter: Parameter '%s' does not exist", _ParameterName.c_str());
	return 0;
}

EffectParameter* Effect::GetParameterBySemantic( const std::string& _Semantic )
{
	// First attempt to find the semantic parameter in the 'cache' map.
	ParametersConstIter it = m_ParametersBySemantic.find(_Semantic);
	if (it != m_ParametersBySemantic.end())
	{
		return (*it).second;
	}

	// No existing semantic parameter found, try searching for it because it may have just
	// not been cached yet.
	CGparameter param = cgGetEffectParameterBySemantic((CGeffect)m_CGEffect, _Semantic.c_str());
	if (param != 0)
	{
		EffectParameter* effectparameter = new EffectParameter(param, _Semantic);
		m_Parameters.insert(ParametersPair(_Semantic, effectparameter));
		return effectparameter;
	}

	LOG(VL_ERROR, "Effect::GetParameterBySemantic: Parameter with semantic '%s' does not exists", _Semantic.c_str());
	return 0;
}

EffectTechnique* Effect::GetTechnique( const std::string& _TechniqueName )
{
	TechniquesConstIter it = m_Techniques.find(_TechniqueName);
	if (it != m_Techniques.end())
	{
		return (*it).second;
	}
	LOG(VL_ERROR, "Effect::GetTechnique: Technique '%s' does not exist (Did it fail validation?)", _TechniqueName.c_str());
	return 0;
}

void Effect::SetActiveTechnique( const std::string& _TechniqueName )
{
	EffectTechnique* technique = GetTechnique(_TechniqueName);
	if (!technique)
	{
		LOG(VL_ERROR, "Effect::SetActiveTechnique: Technique '%s' does not exist (Dit it fail validation?)", _TechniqueName.c_str());
		return;
	}
	SetActiveTechnique(technique);
}

void Effect::SetActiveTechnique( EffectTechnique* _Technique )
{
	assert(_Technique != 0);
	m_ActiveTechnique = _Technique;
}

void Effect::_LoadTechniques()
{
	CGtechnique technique = cgGetFirstTechnique((CGeffect)m_CGEffect);
	while (technique)
	{
		std::string techniquename = std::string(cgGetTechniqueName(technique));
		
		if (cgValidateTechnique(technique))
		{
			EffectTechnique* tech = new EffectTechnique((void*)technique, techniquename);
			m_Techniques.insert(TechniquesPair(techniquename, tech));
		}
		else
		{
			LOG(VL_ERROR, "Effect::_LoadTechniques: Technique '%s' failed to validate.", techniquename.c_str());
		}

		technique = cgGetNextTechnique(technique);
	}

	if (m_Techniques.size() <= 0)
	{
		LOG(VL_ERROR, "Effect: Could not find any valid techniques for effect '%s'", m_Name.c_str());
		m_HasValidTechnique = false;
	}
	else
	{
		m_ActiveTechnique = (*m_Techniques.begin()).second;
		m_HasValidTechnique = true;
	}
}

void Effect::_LoadNamedParameters()
{
	CGparameter param = cgGetFirstEffectParameter((CGeffect)m_CGEffect);
	while (param)
	{
		std::string paramname = std::string(cgGetParameterName(param));
		m_Parameters.insert(ParametersPair(paramname, new EffectParameter(param, paramname)));

		param = cgGetNextParameter(param);
	}
}

bool Effect::HasNextPass()
{
	if (m_CGCurrentPass == 0)
	{
		m_CGCurrentPass = cgGetFirstPass((CGtechnique)m_ActiveTechnique->m_CGTechnique);

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