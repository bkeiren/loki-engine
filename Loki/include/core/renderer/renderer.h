/*
	Deferred rendering pipeline was set up with a huge amount of help from 
	http://www.codinglabs.net/tutorial_simple_def_rendering.aspx (Accessed 14-12-2011 @ 19:45)

	If you ever read this: Thank you very much! :D
*/

#pragma once

#define DBG_VISUALIZATIONS

#ifndef RENDERBASE_H
#define RENDERBASE_H

#include "core/renderer/GLSLShader/glslshader.h"
#include "core/renderer/debugrenderer.h"	// So that files including renderer.h can also use the debug suite.
#include <AssImp//assimp.h>
#include "core/renderer/enums.h"

namespace loki
{

namespace game
{
	class LkLevel;
}

class LkWindow;

namespace renderer
{

class LkScene;
class LkDeferredRenderObject;
class LkEffect;
class LkFramebufferObject;

/*
	The Renderer class is responsible for keeping track of the renderable scene
*/
class LkRenderer
{
public:
	LkRenderer( LkWindow* _Window );
	LkRenderer();
	virtual ~LkRenderer();

	//static void SetScene( Scene* _Scene );
	//static Scene* GetScene();
	void Render( game::LkLevel* _Level );

	void ToggleWireframe();

	void CheckGLError();

#ifdef DBG_VISUALIZATIONS
	void ToggleVisualizeRenderTargets();
	void ToggleVisualizeLightVolumes();
#endif

	//////////////////////////////////////////////////////////////////////////
	// Draws a buffer of pixel data to the screen.
	// TODO: Extend functionality so that the gl type of the buffer can also
	// be chosen by the client (Currently, it is hardcoded to 
	// GL_UNSIGNED_BYTE).
	//////////////////////////////////////////////////////////////////////////
	void DrawPixels( int _Width, int _Height, EInternalFormat _Format, const void* _Buffer );

	int GetWindowWidth();
	int GetWindowHeight();
	int GetRenderWidth();
	int GetRenderHeight();
	vec2 GetPixelScale();
private:
	bool _Init( LkWindow* _Window );
	void _Shutdown();

	void _BindGLFunctions();

	void _RenderSky();					// Renders the sky.
	void _RenderOpaqueGeometry();			// Renders opaque geometry.
	void _RenderLighting();				// Renders lights and writes to the accumulation buffer.
	void _RenderTransparentGeometry();	// Renders transparent geometry.
	//static void RenderPreUIPostProcess();		// Renders pre-ui post processing.
	void _RenderUIObjects();				// Renders UI objects that are not part of the standard UI.
	void _RenderUI();						// Renders the standard UI.
	void _RenderParticles();

	void _RenderLightingPointLights();
	void _RenderLightingDirectionalLights();
	void _RenderLightingSpotLights();

	void _RenderLightAccumulationToBackBuffer();

	void _RenderGBufferTargets();

	GLint m_PointLightShaderColorID;
	GLint m_PointLightShaderPositionID;
	GLint m_PointLightShaderRadiusID;
	GLint m_PointLightShaderScreenDimensionsID;
	GLint m_PointLightShaderRT0ID;
	GLint m_PointLightShaderRT1ID;
	GLint m_PointLightShaderRT2ID;
	GLint m_PointLightShaderRTDepthID;

	GLint m_DirectionalLightShaderColorID;
	GLint m_DirectionalLightShaderDirectionID;
	GLint m_DirectionalLightShaderScreenDimensionsID;
	GLint m_DirectionalLightShaderRT0ID;
	GLint m_DirectionalLightShaderRT1ID;
	GLint m_DirectionalLightShaderRT2ID;
	GLint m_DirectionalLightShaderRTDepthID;

	//static Scene* m_Scene;
	LkWindow* m_Window;

	game::LkLevel* m_CurrentLevelToRender;	// TODO: Probably don't want to do things this way...

	aiLogStream m_AssImpLogStream;

	LkEffect* m_LightEffect;
	LkEffect* m_LightAccumulationToBackBufferEffect;

	// Visualization of Gbuffer targets for debug purposes.
	LkEffect* m_GBufferTargets_General;
	LkEffect* m_GBufferTargets_Normals;
	LkEffect* m_GBufferTargets_Depth;

	LkFramebufferObject* m_GBuffer;

#ifdef DBG_VISUALIZATIONS
	bool m_DBG_VisualizeGBufferTargets;
	bool m_DBG_VisualizeLightVolumes;
#endif
};

extern LkRenderer* g_Renderer;

#define CheckGL()	g_Renderer->CheckGLError()

}

}

#endif