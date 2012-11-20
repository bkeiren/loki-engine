#pragma once

#ifndef EFFECT_H
#define EFFECT_H

#include <hash_map>
#include <string>

namespace loki
{

namespace graphics
{

class EffectParameter;
class EffectTechnique;

class Effect
{
	friend class EffectManager;

	CONTAINER_MACRO_HASH_MAP(std::string, EffectParameter*, Parameters)
	CONTAINER_MACRO_HASH_MAP(std::string, EffectTechnique*, Techniques)
public:
	//////////////////////////////////////////////////////////////////////////
	// Get this effect's name.
	const std::string& GetName() const;

	//////////////////////////////////////////////////////////////////////////
	// Find a shader parameter.
	EffectParameter* GetParameter( const std::string& _ParameterName );
	EffectParameter* GetParameterBySemantic( const std::string& _Semantic );

	//////////////////////////////////////////////////////////////////////////
	// Used to activate the effect and iterate over all passes.
	// Example use:
	// while (effect->HasNextPass())
	// {
	//		RenderGeometry();	// HasNextPass() has already called cgSetPassState().
	// }
	bool HasNextPass();
private:
	Effect( void* _Effect, const std::string& _EffectName );
	Effect();
	~Effect();

	void _LoadNamedParameters();

	std::string m_Name;
	void* m_CGEffect;
	void* m_CGTechnique;
	void* m_CGCurrentPass;
	Parameters m_Parameters;
	Parameters m_ParametersBySemantic;
	bool m_HasValidTechnique;
	Techniques m_Techniques;
};

}

}

#endif