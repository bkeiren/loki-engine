#include "core/graphics/RenderBuffer.h"
#include "glew/glew.h"

namespace loki
{

namespace graphics
{

namespace
{

//#define USE_EXT_DEPTH_STENCIL	// If defined, certain OpenGL defined preprocessor defines will have _EXT appended to them in order to use 
								// the extension version instead of core.
								// Example: RBIF_DEPTH_STENCIL is defined to GL_DEPTH_STENCIL without USE_EXT_DEPTH_STENCIL, but it's defined
								// to GL_DEPTH_STENCIL_EXT *with* USE_EXT_DEPTH_STENCIL.

#ifdef USE_EXT_DEPTH_STENCIL
#define EXT(v)	v##_EXT
#else
#define EXT(v)	v
#endif

GLenum GLInternalFormats[RenderBuffer::_INTERNAL_FORMAT_COUNT] =	{ GL_DEPTH_COMPONENT,
																	  GL_DEPTH_COMPONENT16,
																	  GL_DEPTH_COMPONENT24,
																	  GL_DEPTH_COMPONENT32,
																	  EXT(GL_DEPTH_STENCIL),
																	  EXT(GL_DEPTH24_STENCIL8),
																	  GL_LUMINANCE,
																	  GL_LUMINANCE4,
																	  GL_LUMINANCE8,
																	  GL_LUMINANCE12,
																	  GL_LUMINANCE16,
																	  GL_LUMINANCE_ALPHA,
																	  GL_LUMINANCE4_ALPHA4,
																	  GL_LUMINANCE6_ALPHA2,
																	  GL_LUMINANCE8_ALPHA8,
																	  GL_LUMINANCE12_ALPHA4,
																	  GL_LUMINANCE12_ALPHA12,
																	  GL_LUMINANCE16_ALPHA16,
																	  GL_INTENSITY,
																	  GL_INTENSITY4,
																	  GL_INTENSITY8,
																	  GL_INTENSITY12,
																	  GL_INTENSITY16,
																	  GL_R3_G3_B2,
																	  GL_RGB,
																	  GL_RGB4,
																	  GL_RGB5,
																	  GL_RGB8,
																	  GL_RGB10,
																	  GL_RGB12,
																	  GL_RGB16,
																	  GL_RGB32F,
																	  GL_RGBA,
																	  GL_RGBA12,
																	  GL_RGBA4,
																	  GL_RGB5_A1,
																	  GL_RGBA8,
																	  GL_RGB10_A2,
																	  GL_RGBA12,
																	  GL_RGBA16,
																	  GL_RGBA32F };

#ifdef EXT
#undef EXT
#endif

}

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
