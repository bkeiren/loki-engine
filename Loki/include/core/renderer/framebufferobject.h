#pragma once

#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

#include "core/renderer/renderbufferinfo.h"

namespace loki
{

namespace renderer
{

struct LkRenderBufferInfo;

enum ERenderBufferStorageType
{
	RBST_TEXTURE = 0,
	RBST_BUFFER
};

class LkFramebufferObject
{
public:
	typedef std::pair<ERenderBufferStorageType, GLuint>		RenderBufferVectorPair;
	typedef std::vector<RenderBufferVectorPair>				RenderBufferVector;
	typedef RenderBufferVector::iterator					RenderBufferVectorIter;
	typedef RenderBufferVector::const_iterator				RenderBufferVectorConstIter;

	LkFramebufferObject( unsigned int _Width, unsigned int _Height, const std::vector<LkRenderBufferInfo>& _RenderBuffersInfo );
	~LkFramebufferObject();

	//////////////////////////////////////////////////////////////////////////
	// Returns the OpenGL handle for the render buffer at index _Index.
	//////////////////////////////////////////////////////////////////////////
	unsigned int GetRenderbufferTexture( unsigned int _Index );

	bool CheckFramebufferStatus();

	void Bind();
	
	void Unbind();
	
	//////////////////////////////////////////////////////////////////////////
	// Assumes the frame buffer is bound.
	//////////////////////////////////////////////////////////////////////////
	void SetDrawBuffers( EFrameBufferAttachment* _Buffers, unsigned int _NumBuffers );
	void SetDrawBuffer( EFrameBufferAttachment _Buffer );
	
	void SetClearColor( const vec4& _Color );
	
	void SetClearDepth( float _Depth );
	
	void SetClearStencil( int _Stencil );

	//////////////////////////////////////////////////////////////////////////
	// Assumes the frame buffer is bound.
	//////////////////////////////////////////////////////////////////////////
	void ClearBuffers( int _ClearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER | GL_STENCIL_BUFFER_BIT );

	int GetWidth() const;
	int GetHeight() const;
private:
	LkFramebufferObject();

	GLuint m_FBO;
	RenderBufferVector m_RenderBuffers;	// First element of the pair indicates whether the second element
																				// defines a render buffer handle or a texture handle.
	unsigned int m_Width;
	unsigned int m_Height;
	vec4 m_ClearColor;
	float m_ClearDepth;
	int m_ClearStencil;
};

}

}

#endif