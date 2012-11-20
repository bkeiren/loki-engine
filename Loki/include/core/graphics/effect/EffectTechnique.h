#pragma once

#ifndef EFFECTTECHNIQUE_H
#define EFFECTTECHNIQUE_H

namespace loki
{

namespace graphics
{

class EffectTechnique
{
	friend class Effect;
public:
	const std::string& GetName() const;

private:
	EffectTechnique( void* _Technique, const std::string& _TechniqueName );
	EffectTechnique();
	~EffectTechnique();

	std::string m_Name;
	void* m_CGTechnique;
};

}

}

#endif
