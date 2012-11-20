#include "core/graphics/effect/EffectManager.h"
#include "core/graphics/effect/Effect.h"

#include <Windows.h>

#include <iostream>
#include <string>
#include <sstream>

#include "Cg/cgGL.h"

namespace loki
{

namespace graphics
{

EffectManager* g_EffectManager = NULL;

namespace
{
	// Error handler for Cg
	void CgErrorHandler( CGcontext context, CGerror error, void* appdata )
	{
		if ( error != CG_NO_ERROR )
		{
			std::stringstream ss;
			const char* pStr = cgGetErrorString(error);
			std::string strError = ( pStr == NULL ) ? "" : pStr;
			ss << "Cg: " << strError << std::endl;

			std::string strListing;
			if ( error == CG_COMPILER_ERROR )
			{
				pStr = cgGetLastListing( context );
				strListing = ( pStr == NULL ) ? "" : pStr;

				ss << strListing << std::endl;
			}
// #ifdef _WIN32
// 			OutputDebugStringA( ss.str().c_str() );
// #else
// 			std::cerr << ss;
// #endif
			LOG(VL_ERROR, ss.str().c_str());
		}
	}
}

EffectManager::EffectManager()
{
	_Init();
}

EffectManager::~EffectManager()
{
	_Shutdown();
}

void EffectManager::_Init()
{
	cgSetErrorHandler(&CgErrorHandler, NULL);
	m_CGContext = cgCreateContext();

	cgGLRegisterStates((CGcontext)m_CGContext);
	cgGLSetManageTextureParameters((CGcontext)m_CGContext, CG_TRUE);
	
	LOG(VL_ALWAYS, "EffectManager::Init: Effectmanager initialized");
}

void EffectManager::_Shutdown()
{
	for (EffectsConstIter it = m_Effects.begin(); it != m_Effects.end(); ++it)
	{
		delete (*it).second;
	}

	if (m_CGContext != NULL)
	{
		cgDestroyContext((CGcontext)m_CGContext);
		m_CGContext = NULL;
	}
	cgSetErrorHandler(NULL, NULL);

	LOG(VL_ALWAYS, "EffectManager::Shutdown: Effectmanager terminated");
}

Effect* EffectManager::CreateEffectFromFile( const std::string& _File, const std::string& _EffectName )
{
	if (GetEffect(_EffectName))
	{
		LOG(VL_ERROR, "EffectManager::CreateEffectFromFile: An effect with name %s already exists", _EffectName);
		return 0;
	}

	CGeffect cgeffect = cgCreateEffectFromFile((CGcontext)m_CGContext, _File.c_str(), NULL);
	if (cgeffect == NULL)
	{
		//std::string temp = cgGetLastListing((CGcontext)m_CGContext);	// Stored in a temporary variable because using it directly crashes
																		// in the logger.
		LOG(VL_ERROR, "EffectManager::CreateEffectFromFile: Failed to load effect from file");
		return 0;
	}

	Effect* effect = new Effect((void*)cgeffect, _EffectName);
	m_Effects.insert(EffectsPair(_EffectName, effect));
	return effect;
}

Effect* EffectManager::CreateEffectFromMemory( const std::string& _Source, const std::string& _EffectName )
{
	if (GetEffect(_EffectName))
	{
		LOG(VL_ERROR, "EffectManager::CreateEffectFromMemory: An effect with name %s already exists", _EffectName);
		return 0;
	}

	CGeffect cgeffect = cgCreateEffect((CGcontext)m_CGContext, _Source.c_str(), NULL);
	if (cgeffect == NULL)
	{
		std::string temp = cgGetLastListing((CGcontext)m_CGContext);	// Stored in a temporary variable because using it directly crashes
		// in the logger.
		LOG(VL_ERROR, "EffectManager::CreateEffectFromMemory: Failed to load effect from file:\n%s", temp.c_str());
		return 0;
	}

	Effect* effect = new Effect((void*)cgeffect, _EffectName);
	m_Effects.insert(EffectsPair(_EffectName, effect));
	return effect;
}

Effect* EffectManager::GetEffect( const std::string& _EffectName ) const
{
	EffectsConstIter it = m_Effects.find(_EffectName);
	if (it != m_Effects.end())
	{
		return (*it).second;
	}
	return 0;
}

// void SetViewMatrix( const mat4& _M )
// {
// 
// }
// 
// void SetModelMatrix( const mat4& _M )
// {
// 
// }
// 
// void SetProjectionMatrix( const mat4& _M )
// {
// 
// }

}

}