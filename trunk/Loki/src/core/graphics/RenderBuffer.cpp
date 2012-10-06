#include "core/graphics/RenderBuffer.h"
#include "glew/glew.h"

namespace loki
{

namespace graphics
{

RenderBuffer::RenderBuffer( EInternalFormat _InternalFormat, int32 _Width, int32 _Height )	:
	m_InternalFormat(_InternalFormat),
	m_Width(_Width),
	m_Height(_Height)
{
	glGenRenderbuffers(1, &m_GLBufferHandle);

	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GLInternalFormats[m_InternalFormat], m_Width, m_Height);
	Unbind();
}

RenderBuffer::RenderBuffer()
{
	ILLEGAL_CTOR_ERROR("RenderBuffer");
}

RenderBuffer::~RenderBuffer()
{
	glDeleteRenderbuffers(1, &m_GLBufferHandle);
}

RenderBuffer* RenderBuffer::Create( EInternalFormat _InternalFormat, int32 _Width, int32 _Height )
{
	RenderBuffer* rb = new RenderBuffer(_InternalFormat, _Width, _Height);

	return rb;
}

uint32 RenderBuffer::GetBufferHandle() const
{
	return m_GLBufferHandle;
}

void RenderBuffer::Bind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_GLBufferHandle);
}

void RenderBuffer::Unbind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

}

}
