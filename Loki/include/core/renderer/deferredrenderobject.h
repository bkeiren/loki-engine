/*
	Deferred rendering pipeline was set up with a huge amount of help from 
	http://www.codinglabs.net/tutorial_simple_def_rendering.aspx (Accessed 14-12-2011 @ 19:45)

	If you ever read this: Thank you very much! :D
*/

#pragma once

#ifndef DEFERREDRENDERING_H
#define DEFERREDRENDERING_H

#include <GLEW\\glew.h>
#include "core/renderer/glslshader/glslshader.h"

namespace loki
{

namespace renderer
{

class LkMRTObject;

class LkDeferredRenderObject
{
public:
	LkDeferredRenderObject( int _Width, int _Height, LkMRTObject* _MultipleRenderTargetObject );
	~LkDeferredRenderObject();

	void Render();

	void ToggleVisualizeRenderTargets();
private:
	LkGLSLShader m_Shader;	// The shader to be used to assemble all deferred rendering data into 
							// a single rendered image.
	LkMRTObject* m_MRTObject;	// A pointer to the FBO object that contains diffuse, normals and positions.

	unsigned int m_Width;
	unsigned int m_Height;

	LkGLSLShader m_DebugShaderDiffuse;		// These shaders are used to visualize the different render targets
	LkGLSLShader m_DebugShaderPosition;	// of the G-buffer. Each shader simply renders one of the buffers.
	LkGLSLShader m_DebugShaderNormal;		// It can be used to render the classical 4-view window in which each view
	LkGLSLShader m_DebugShaderDepth;		// features a different render target's contents.
	LkGLSLShader m_DebugShaderLight;
	GLint m_DebugDiffuseID;
	GLint m_DebugPositionID;
	GLint m_DebugNormalsID;
	GLint m_DebugDepthID;
	GLint m_DebugLightID;


	GLint m_RT0_ID;		// RT0 texture handle in the shader. Is obtained with glGetUniformLocationARB().
	GLint m_RT1_ID;		// RT1 texture handle in the shader. Is obtained with glGetUniformLocationARB().
	GLint m_RT2_ID;		// RT2 texture handle in the shader. Is obtained with glGetUniformLocationARB().
	GLint m_RTDepth_ID;
	GLint m_LightingID;	// Accumulation texture handle in the shader.
	GLint m_ScreenDimensionsID;

	bool m_VisualizeRenderTargets;
};

}

}

#endif