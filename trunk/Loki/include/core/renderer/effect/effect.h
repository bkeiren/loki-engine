#pragma once

#ifndef EFFECT_H
#define EFFECT_H

#include <hash_map>
#include <string>

namespace loki
{

namespace renderer
{

class LkEffectParameter;

class LkEffect
{
	friend class LkEffectManager;

	typedef stdext::hash_map<std::string, LkEffectParameter*>	Parameters;
	typedef Parameters::iterator								ParametersItr;
	typedef Parameters::const_iterator							ParametersConstItr;
	typedef std::pair<std::string, LkEffectParameter*>			ParametersPair;
public:
	//////////////////////////////////////////////////////////////////////////
	// Get this effect's name.
	const std::string& GetName() const;

	//////////////////////////////////////////////////////////////////////////
	// Find a shader parameter.
	LkEffectParameter* GetParameter( const std::string& _ParameterName );
	LkEffectParameter* GetParameterBySemantic( const std::string& _Semantic );

	//////////////////////////////////////////////////////////////////////////
	// Used to activate the effect and iterate over all passes.
	// Example use:
	// while (effect->HasNextPass())
	// {
	//		RenderGeometry();	// HasNextPass() has already called cgSetPassState().
	// }
	bool HasNextPass();
private:
	LkEffect( void* _Effect, const std::string& _EffectName );
	LkEffect();
	~LkEffect();

	void _LoadNamedParameters();

	std::string m_Name;
	void* m_CGEffect;
	void* m_CGTechnique;
	void* m_CGCurrentPass;
	Parameters m_Parameters;
	Parameters m_ParametersBySemantic;
	bool m_HasValidTechnique;
};

}

}

#endif