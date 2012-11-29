#include "core/graphics/effect/EffectManager.h"
//#include "core/graphics/effect/Effect.h"

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

	_UnregisterSharedParameters();

	if (m_CGContext != NULL)
	{
		cgDestroyContext((CGcontext)m_CGContext);
		m_CGContext = NULL;
	}
	cgSetErrorHandler(NULL, NULL);

	LOG(VL_ALWAYS, "EffectManager::Shutdown: Effectmanager terminated");
}

void EffectManager::_RegisterDefaultSharedParameters()
{
#define CREATE_SHARED_PARAMETER(NAME, TYPE)									\
	{	CGparameter p = cgCreateParameter((CGcontext)m_CGContext, TYPE);	\
		m_SharedParameters[NAME] = new EffectParameter((void*)p, NAME);		}

	CREATE_SHARED_PARAMETER("LKPROJMATRIX", CG_FLOAT4x4);
	CREATE_SHARED_PARAMETER("LKVIEWMATRIX", CG_FLOAT4x4);
	CREATE_SHARED_PARAMETER("LKMODELMATRIX", CG_FLOAT4x4);
	CREATE_SHARED_PARAMETER("LKMODELMATRIXIT", CG_FLOAT3x3);
	CREATE_SHARED_PARAMETER("LKMODELVIEWMATRIX", CG_FLOAT4x4);
	CREATE_SHARED_PARAMETER("LKVIEWPROJMATRIX", CG_FLOAT4x4);
	CREATE_SHARED_PARAMETER("LKMODELVIEWPROJMATRIX", CG_FLOAT4x4);
	CREATE_SHARED_PARAMETER("LKZNEAR", CG_FLOAT);
	CREATE_SHARED_PARAMETER("LKZFAR", CG_FLOAT);
	CREATE_SHARED_PARAMETER("LKCAMERAPOSITION", CG_FLOAT3);
	CREATE_SHARED_PARAMETER("LKTEXDIFFUSE", CG_SAMPLER2D);
	CREATE_SHARED_PARAMETER("LKTEXNORMAL", CG_SAMPLER2D);
	CREATE_SHARED_PARAMETER("LKTEXSPECULAR", CG_SAMPLER2D);
	CREATE_SHARED_PARAMETER("LKTEXEMISSIVE", CG_SAMPLER2D);
	CREATE_SHARED_PARAMETER("LKTEXENV", CG_SAMPLER3D);
	CREATE_SHARED_PARAMETER("LKLIGHTPOSITION", CG_FLOAT3);
	CREATE_SHARED_PARAMETER("LKLIGHTRANGE", CG_FLOAT);
	CREATE_SHARED_PARAMETER("LKLIGHTVECTOR", CG_FLOAT3);
	CREATE_SHARED_PARAMETER("LKLIGHTCOLOR", CG_FLOAT3);
	CREATE_SHARED_PARAMETER("LKLIGHTINTENSITY", CG_FLOAT);
	CREATE_SHARED_PARAMETER("LKLIGHTTYPE", CG_INT);
	CREATE_SHARED_PARAMETER("LKIGHTSPOTANGLE", CG_FLOAT);
	CREATE_SHARED_PARAMETER("LKLIGHTRANGE", CG_FLOAT);
	CREATE_SHARED_PARAMETER("LKTEXLIGHTPOINT", CG_SAMPLER2D);
	CREATE_SHARED_PARAMETER("LKTEXLIGHTSPOT", CG_SAMPLER2D);
	CREATE_SHARED_PARAMETER("LKTEXGBUFFER0", CG_SAMPLER2D);
	CREATE_SHARED_PARAMETER("LKTEXGBUFFER1", CG_SAMPLER2D);
	CREATE_SHARED_PARAMETER("LKTEXGBUFFER2", CG_SAMPLER2D);
	CREATE_SHARED_PARAMETER("LKTEXGBUFFER3", CG_SAMPLER2D);
	CREATE_SHARED_PARAMETER("LKUVSCALE", CG_FLOAT2);
	CREATE_SHARED_PARAMETER("LKMATERIALSHININESS", CG_FLOAT);
	CREATE_SHARED_PARAMETER("LKMATERIALREFLECTIVITY", CG_FLOAT);
	CREATE_SHARED_PARAMETER("LKCOOKIEPOINT", CG_SAMPLERCUBE);
	CREATE_SHARED_PARAMETER("LKCOOKIESPOT", CG_SAMPLER2D);

#undef CREATE_SHARED_PARAMETER
}

void EffectManager::_UnregisterSharedParameters()
{
	for (SharedParametersIter it = m_SharedParameters.begin(); it != m_SharedParameters.end(); ++it)
	{
		cgDestroyParameter((CGparameter)it->second->m_CGParameter);
		delete it->second;
	}
	m_SharedParameters.clear();
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