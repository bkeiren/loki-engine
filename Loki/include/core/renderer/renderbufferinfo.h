#pragma once

#ifndef RENDERBUFFERINFO_H
#define RENDERBUFFERINFO_H

#include <vector>
#include "core/renderer/enums.h"

namespace loki
{

namespace renderer
{

struct LkRenderBufferInfo
{
	friend class LkFramebufferObject;

	LkRenderBufferInfo();
	~LkRenderBufferInfo();

	EInternalFormat m_InternalFormat;

	//////////////////////////////////////////////////////////////////////////
	// Only applies if m_GenerateTexture is true.
	//////////////////////////////////////////////////////////////////////////
	ETextureFormat m_TextureFormat;

	//////////////////////////////////////////////////////////////////////////
	// Only applies if m_GenerateTexture is true.
	//////////////////////////////////////////////////////////////////////////
	ETextureType m_TextureType;

	//////////////////////////////////////////////////////////////////////////
	// Only applies if m_GenerateTexture is true.
	//////////////////////////////////////////////////////////////////////////
	const void* m_TexturePixels;

	EAttachment m_Attachment;
	
	bool m_GenerateTexture;	// If true, will generate a texture instead of using a standard OpenGL render buffer.

	// TODO: Provide own enum for _Enum instead of using GL_ defines.
	void AddTextureParameter( GLenum _Enum, GLint _Value );
	void AddTextureParameter( GLenum _Enum, GLfloat _Value );
private:
	std::vector<std::pair<GLenum, GLint> > m_TextureParametersInteger;
	std::vector<std::pair<GLenum, GLfloat> > m_TextureParametersFloat;
};

}

}

#endif