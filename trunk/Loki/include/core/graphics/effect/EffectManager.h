#pragma once

#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include <hash_map>
#include "core/graphics/effect/Effect.h"
#include "core/graphics/effect/EffectParameter.h"

namespace loki
{

namespace renderer
{
	class LkRenderer;
}

namespace graphics
{

class Effect;

class EffectManager
{
	CONTAINER_MACRO_HASH_MAP(std::string, Effect*, Effects)

	friend class renderer::LkRenderer;
public:
	//////////////////////////////////////////////////////////////////////////
	// Create an effect from a .cgfx file.
	Effect* CreateEffectFromFile( const std::string& _File, const std::string& _EffectName );

	//////////////////////////////////////////////////////////////////////////
	// Create an effect from a string of source code.
	Effect* CreateEffectFromMemory( const std::string& _Source, const std::string& _EffectName );

	//////////////////////////////////////////////////////////////////////////
	// Find an already created effect by name.
	Effect* GetEffect( const std::string& _EffectName ) const;

// 	void SetViewMatrix( const mat4& _M );
// 	void SetModelMatrix( const mat4& _M );
// 	void SetProjectionMatrix( const mat4& _M );
private:
	EffectManager();
	~EffectManager();

	void _Init();
	void _Shutdown();

	Effects m_Effects;
	//CGcontext m_CGContext;
	void* m_CGContext;
};

extern EffectManager* g_EffectManager;

}

}

#endif