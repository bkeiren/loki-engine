#pragma once

#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include "core/graphics/Enums.h"

namespace loki
{

namespace graphics
{

class RenderBuffer
{
public:
	~RenderBuffer();

	static RenderBuffer* Create( EInternalFormat _InternalFormat, int32 _Width, int32 _Height );

	uint32 GetBufferHandle() const;

	void Bind() const;
	void Unbind() const;

private:
	RenderBuffer( EInternalFormat _InternalFormat, int32 _Width, int32 _Height );
	RenderBuffer();

	uint32 m_GLBufferHandle;
	EInternalFormat m_InternalFormat;
	int32 m_Width;
	int32 m_Height;
};

}

}

#endif