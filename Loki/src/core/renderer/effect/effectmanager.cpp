#include "core/renderer/effect/effectmanager.h"
#include "core/renderer/effect/effect.h"

#include <Windows.h>

#include <iostream>
#include <string>
#include <sstream>

#include "Cg/cgGL.h"

namespace loki
{

namespace renderer
{

LkEffectManager* g_EffectManager = NULL;

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

LkEffectManager::LkEffectManager()
{
	_Init();
}

LkEffectManager::~LkEffectManager()
{
	_Shutdown();
}

void LkEffectManager::_Init()
{
	cgSetErrorHandler(&CgErrorHandler, NULL);
	m_CGContext = cgCreateContext();

	cgGLRegisterStates((CGcontext)m_CGContext);
	cgGLSetManageTextureParameters((CGcontext)m_CGContext, CG_TRUE);
	
	LOG(VL_ALWAYS, "EffectManager::Init: Effectmanager initialized");
}

void LkEffectManager::_Shutdown()
{
	for (EffectsConstItr it = m_Effects.begin(); it != m_Effects.end(); ++it)
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

LkEffect* LkEffectManager::CreateEffectFromFile( const std::string& _File, const std::string& _EffectName )
{
	if (GetEffect(_EffectName))
	{
		LOG(VL_ERROR, "EffectManager::CreateEffectFromFile: An effect with name %s already exists", _EffectName);
		return 0;
	}

	CGeffect cgeffect = cgCreateEffectFromFile((CGcontext)m_CGContext, _File.c_str(), NULL);
	if (cgeffect == NULL)
	{
		std::string temp = cgGetLastListing((CGcontext)m_CGContext);	// Stored in a temporary variable because using it directly crashes
																		// in the logger.
		LOG(VL_ERROR, "EffectManager::CreateEffectFromFile: Failed to load effect from file:\n%s", temp.c_str());
		return 0;
	}

	LkEffect* effect = new LkEffect((void*)cgeffect, _EffectName);
	m_Effects.insert(EffectsPair(_EffectName, effect));
	return effect;
}

LkEffect* LkEffectManager::CreateEffectFromMemory( const std::string& _Source, const std::string& _EffectName )
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

	LkEffect* effect = new LkEffect((void*)cgeffect, _EffectName);
	m_Effects.insert(EffectsPair(_EffectName, effect));
	return effect;
}

LkEffect* LkEffectManager::GetEffect( const std::string& _EffectName ) const
{
	EffectsConstItr it = m_Effects.find(_EffectName);
	if (it != m_Effects.end())
	{
		return (*it).second;
	}
	return 0;
}

// void SetViewMatrix( const glm::mat4& _M )
// {
// 
// }
// 
// void SetModelMatrix( const glm::mat4& _M )
// {
// 
// }
// 
// void SetProjectionMatrix( const glm::mat4& _M )
// {
// 
// }

}

}