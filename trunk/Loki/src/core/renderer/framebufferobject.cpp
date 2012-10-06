#include "core/renderer/framebufferobject.h"
#include "core/renderer/renderer.h"	// For CheckGL().

#include "core/graphics/RenderBuffer.h"

namespace loki
{

namespace renderer
{

LkFramebufferObject::LkFramebufferObject( unsigned int _Width, unsigned int _Height, const std::vector<LkRenderBufferInfo>& _RenderBuffersInfo )	:
	m_Width(_Width),
	m_Height(_Height),
	m_ClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f)),
	m_ClearDepth(1.0f),
	m_ClearStencil(0),
	m_FBO(0)
{
	//////////////////////////////////////////////////////////////////////////
	// FUTURE REFERENCE NOTE: Some source on the internet claims that
	// certain ATI drivers might crash if you try to add a depth render buffer
	// and then a texture (in that order) to the FBO. While this claim was posted
	// a while ago, it might still hold true and is something to consider.
	//////////////////////////////////////////////////////////////////////////

	assert(m_Width > 0);
	assert(m_Height > 0);

	// Ask OpenGL to create off-screen frame buffer.
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	m_RenderBuffers.resize(_RenderBuffersInfo.size(), RenderBufferVectorPair(RBST_TEXTURE, 0));

	unsigned int i = 0;
	for (std::vector<LkRenderBufferInfo>::const_iterator it = _RenderBuffersInfo.begin(); it != _RenderBuffersInfo.end(); ++it)
	{
		const LkRenderBufferInfo& Info = (*it);

		if (Info.m_GenerateTexture)
		{
			m_RenderBuffers[i].first = RBST_TEXTURE;

			GLuint* Tex = &(m_RenderBuffers[i].second);

			glGenTextures(1, Tex);
			glBindTexture(GL_TEXTURE_2D, *Tex);
			glTexImage2D(GL_TEXTURE_2D, 0, Info.m_InternalFormat, m_Width, m_Height, 0, Info.m_TextureFormat, Info.m_TextureType, Info.m_TexturePixels);
			CheckGL();

			// Apply texture parameters.
			for (std::vector<std::pair<GLenum, GLint> >::const_iterator it2 = Info.m_TextureParametersInteger.begin(); it2 != Info.m_TextureParametersInteger.end(); ++it2)
			{
				glTexParameteri(GL_TEXTURE_2D, (*it2).first, (*it2).second);
			}
			for (std::vector<std::pair<GLenum, GLfloat> >::const_iterator it2 = Info.m_TextureParametersFloat.begin(); it2 != Info.m_TextureParametersFloat.end(); ++it2)
			{
				glTexParameterf(GL_TEXTURE_2D, (*it2).first, (*it2).second);
			}

			// Attach the texture to the FBO by connecting it to the right attachment point.
			glFramebufferTexture2D(GL_FRAMEBUFFER, Info.m_Attachment, GL_TEXTURE_2D, m_RenderBuffers[i].second, 0);
			CheckGL();
		}
		else
		{
			m_RenderBuffers[i].first = RBST_BUFFER;

			graphics::RenderBuffer* rb = graphics::RenderBuffer::Create((graphics::RenderBuffer::EInternalFormat)Info.m_InternalFormat, m_Width, m_Height);

			m_RenderBuffers[i].second = (GLuint)rb;

			// Ask OpenGL to create a render buffer.
			//glGenRenderbuffers(1, &(m_RenderBuffers[i].second));

			// Bind the render buffer, set it's storage and indicate which attachment point it should connect to.
			//glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffers[i].second);
			
			// Attach the generated render buffer to the FBO by connecting it to the right attachment point.
			//glFramebufferRenderbuffer(GL_FRAMEBUFFER, Info.m_Attachment, GL_RENDERBUFFER, m_RenderBuffers[i].second);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, Info.m_Attachment, GL_RENDERBUFFER, rb->GetBufferHandle());
			//glRenderbufferStorage(GL_RENDERBUFFER, Info.m_InternalFormat, m_Width, m_Height);
			CheckGL();
		}	

		++i;
	}
}

LkFramebufferObject::LkFramebufferObject()
{
	ILLEGAL_CTOR_ERROR("FrameBufferObject");
}

LkFramebufferObject::~LkFramebufferObject()
{
	glDeleteFramebuffers(1, &m_FBO);

	for (RenderBufferVectorIter it = m_RenderBuffers.begin(); it != m_RenderBuffers.end(); ++it)
	{
		if ((*it).first == RBST_TEXTURE)
		{
			glDeleteTextures(1, &((*it).second));
		}
		else
		{
			//glDeleteRenderbuffers(1, &((*it).second));
			delete (graphics::RenderBuffer*)((*it).second);
		}
	}
}

unsigned int LkFramebufferObject::GetRenderbufferTexture( unsigned int _Index )
{
#ifdef _DEBUG
	if (_Index < 0 || _Index >= m_RenderBuffers.size())
	{
		LOG(VL_WARN, "FrameBufferObject::GetRenderbufferTexture: _Index (%i) out of range (%i).", _Index, m_RenderBuffers.size());
		return 0;
	}
#endif
	if (m_RenderBuffers[_Index].first == RBST_BUFFER)
	{
		LOG(VL_WARN, "FrameBufferObject::GetRenderbufferTexture: Requested index has an OpenGL render buffer associated instead of a shareable texture");
		return 0;
	}
	return m_RenderBuffers[_Index].second;
}

bool LkFramebufferObject::CheckFramebufferStatus()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	bool ret = false;
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		{
			ret = true;
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		{
			LOG(VL_ERROR, "FrameBufferObject::CheckFramebufferStatus: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			//throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		{
			LOG(VL_ERROR, "FrameBufferObject::CheckFramebufferStatus: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			//throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		{
			LOG(VL_ERROR, "FrameBufferObject::CheckFramebufferStatus: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			//throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		{
			LOG(VL_ERROR, "FrameBufferObject::CheckFramebufferStatus: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			//throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			break;
		}
	case GL_FRAMEBUFFER_UNSUPPORTED:
		{
			LOG(VL_ERROR, "FrameBufferObject::CheckFramebufferStatus: GL_FRAMEBUFFER_UNSUPPORTED");
			//throw new std::exception("GL_FRAMEBUFFER_UNSUPPORTED");
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		{
			LOG(VL_ERROR, "FrameBufferObject::CheckFramebufferStatus: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
			//throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		{
			LOG(VL_ERROR, "FrameBufferObject::CheckFramebufferStatus: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
			//throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
			break;
		}
	default:
		{
			LOG(VL_WARN, "FrameBufferObject::CheckFramebufferStatus: Unhandled error (Framebuffer status is %i)", (int)status);
			break;
		}
	}

	// Unbind the frame buffer object from future operations.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return ret;
}

void LkFramebufferObject::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void LkFramebufferObject::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LkFramebufferObject::SetDrawBuffers( EAttachment* _Buffers, unsigned int _NumBuffers )
{
	glDrawBuffers(_NumBuffers, (GLenum*)_Buffers);
}

void LkFramebufferObject::SetDrawBuffer( EAttachment _Buffer )
{
	glDrawBuffer(_Buffer);
}

void LkFramebufferObject::SetClearColor( const vec4& _Color )
{
	m_ClearColor = _Color;
}

void LkFramebufferObject::SetClearDepth( float _Depth )
{
	m_ClearDepth = _Depth;
}

void LkFramebufferObject::SetClearStencil( int _Stencil )
{
	m_ClearStencil = _Stencil;
}

void LkFramebufferObject::ClearBuffers( int _ClearMask /*= GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER | GL_STENCIL_BUFFER_BIT*/ )
{
	Bind();
	glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
	glClearDepth(m_ClearDepth);
	glClearStencil(m_ClearStencil);
	glClear(_ClearMask);
}

int LkFramebufferObject::GetWidth() const
{
	return m_Width;
}

int LkFramebufferObject::GetHeight() const
{
	return m_Height;
}

}

}