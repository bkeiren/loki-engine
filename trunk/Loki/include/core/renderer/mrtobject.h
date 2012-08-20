/*
	Deferred rendering pipeline was set up with a huge amount of help from 
	http://www.codinglabs.net/tutorial_simple_def_rendering.aspx (Accessed 14-12-2011 @ 19:45)
*/
#pragma once

#include <GLEW\\glew.h>
//#include <GLEW\\glext.h>

#ifndef MRTOBJECT_H
#define MRTOBJECT_H

namespace loki
{

namespace renderer
{

class LkEffect;

/*
	The MRTObject class is designed to keep track of the multiple render targets (MRT) that are used for deferred rendering.
*/
class LkMRTObject
{
	friend class LkRenderer;
public:
	LkMRTObject( int _Width, int _Height );
	~LkMRTObject();

	//////////////////////////////////////////////////////////////////////////
	// Sets up OpenGL to use the deferred rendering technique to incoming draw calls.
	// This should be called each frame before submitting scene geometry.
	//////////////////////////////////////////////////////////////////////////
	void StartGBuffer();

	//////////////////////////////////////////////////////////////////////////
	// Stops OpenGL from using the deferred rendering technique.
	// This should be called each frame after submitting scene geometry.
	//////////////////////////////////////////////////////////////////////////
	void StopGBuffer();

	//////////////////////////////////////////////////////////////////////////
	// Binds the light accumulation buffer for rendering.
	// Should be called before starting any lighting rendering so that 
	// the lighting can be blended into the right buffer.
	//////////////////////////////////////////////////////////////////////////
	void StartLightAccumulation();

	//////////////////////////////////////////////////////////////////////////
	// Renders a fullscreen quad in order to get the light accumulation texture
	// on the screen.
	// NOTE: Post processing might be applied here?
	//////////////////////////////////////////////////////////////////////////
	void RenderLightAccumulationToBackBuffer();

	unsigned int GetRT0();
	unsigned int GetRT1();
	unsigned int GetRT2();
	unsigned int GetRTDepthStencil();
	unsigned int GetRTLightAccumulation();
private:
	GLuint m_FBO;					// The Frame Buffer Object ID.
	GLuint m_RT0;					// Render target 0 OpenGL handle.
	GLuint m_RT1;					// Render target 1 OpenGL handle.
	GLuint m_RT2;					// Render target 2 OpenGL handle.
	GLuint m_RTDepth;		// Depth buffer OpenGL handle.
	GLuint m_RTStencil;
	GLuint m_RTLightAccumulation;
	unsigned int m_RT0Texture;		// RT 0 OpenGL texture.
	unsigned int m_RT1Texture;		// RT 1 OpenGL texture.
	unsigned int m_RT2Texture;		// RT 2 OpenGL texture.
	unsigned int m_RTDepthTexture;	// Depth OpenGL texture.
	unsigned int m_RTStencilTexture;
	unsigned int m_RTLightAccumulationTexture;

	unsigned int m_FBOWidth;
	unsigned int m_FBOHeight;

	LkEffect* m_LightAccumulationToBackBufferEffect;
};

}

}

#endif