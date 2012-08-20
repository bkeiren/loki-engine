#pragma once

#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include <hash_map>
#include "core/renderer/effect/effect.h"
#include "core/renderer/effect/effectparameter.h"

namespace loki
{

namespace renderer
{

class LkEffect;

class LkEffectManager
{
	typedef stdext::hash_map<std::string, LkEffect*>	Effects;
	typedef Effects::iterator							EffectsItr;
	typedef Effects::const_iterator						EffectsConstItr;
	typedef std::pair<std::string, LkEffect*>			EffectsPair;

	friend class LkRenderer;
public:
	//////////////////////////////////////////////////////////////////////////
	// Create an effect from a .cgfx file.
	LkEffect* CreateEffectFromFile( const std::string& _File, const std::string& _EffectName );

	//////////////////////////////////////////////////////////////////////////
	// Create an effect from a string of source code.
	LkEffect* CreateEffectFromMemory( const std::string& _Source, const std::string& _EffectName );

	//////////////////////////////////////////////////////////////////////////
	// Find an already created effect by name.
	LkEffect* GetEffect( const std::string& _EffectName ) const;

// 	void SetViewMatrix( const glm::mat4& _M );
// 	void SetModelMatrix( const glm::mat4& _M );
// 	void SetProjectionMatrix( const glm::mat4& _M );
private:
	LkEffectManager();
	~LkEffectManager();

	void _Init();
	void _Shutdown();

	Effects m_Effects;
	//CGcontext m_CGContext;
	void* m_CGContext;
};

extern LkEffectManager* g_EffectManager;

}

}

#endif