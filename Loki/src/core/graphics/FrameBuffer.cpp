#include "core/graphics/FrameBuffer.h"
#include <cassert>
#include "core/graphics/Error.h"
#include "glew/glew.h"
#include "core/graphics/Texture.h"
#include "core/graphics/RenderBuffer.h"

namespace loki
{

namespace graphics
{

FrameBuffer::FrameBufferAttachmentInfo::FrameBufferAttachmentInfo()	:
	m_TexturePixels(0),
	//m_TextureHandle(0),
	m_GenerateTexture(true)
{
	m_InternalFormat = graphics::INTERNAL_FORMAT_RGBA;
	m_TextureFormat = graphics::TEXTURE_FORMAT_RGBA;
	m_TextureType = graphics::TEXTURE_TYPE_UNSIGNED_INT_8_8_8_8;
	m_Attachment = graphics::FRAMEBUFFER_COLOR_ATTACHMENT0;

	{
		// NOTE: If no mipmap levels are generated for the texture (Which is not done by default), then these parameters are required in order to
		// successfully sample the image in certain cases.
		AddTextureParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		AddTextureParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		AddTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		AddTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

FrameBuffer::FrameBufferAttachmentInfo::~FrameBufferAttachmentInfo()
{

}

void FrameBuffer::FrameBufferAttachmentInfo::AddTextureParameter( GLenum _Enum, GLint _Value )
{
	m_TextureParametersInteger.push_back(std::pair<GLenum, GLint>(_Enum, _Value));	
}

void FrameBuffer::FrameBufferAttachmentInfo::AddTextureParameter( GLenum _Enum, GLfloat _Value )
{
	m_TextureParametersFloat.push_back(std::pair<GLenum, GLfloat>(_Enum, _Value));
}

FrameBuffer::FrameBuffer()	:
	m_GLBufferHandle(0),
	m_Width(0),
	m_Height(0)
{
	glGenFramebuffers(1, &m_GLBufferHandle);

	for (int i = 0; i < _FRAMEBUFFER_ATTACHMENT_COUNT; ++i)
	{
		m_Attachments[i] = AttachmentPair(ATTACHMENT_STORAGE_TEXTURE, 0);
	}
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_GLBufferHandle);

	for (int i = 0; i < _FRAMEBUFFER_ATTACHMENT_COUNT; ++i)
	{
		if (m_Attachments[i].second != 0)
		{
			if (m_Attachments[i].first == ATTACHMENT_STORAGE_TEXTURE)
			{
				delete ((Texture*)m_Attachments[i].second);
			}
			else
			{
				delete ((RenderBuffer*)m_Attachments[i].second);
			}
		}
	}
}

FrameBuffer* FrameBuffer::Create( int32 _Width, int32 _Height, const std::vector<FrameBufferAttachmentInfo>& _RenderBuffersInfo )
{
	assert(_Width > 0);
	assert(_Height > 0);

	FrameBuffer* fb = new FrameBuffer();

	fb->m_Width = _Width;
	fb->m_Height = _Height;

	fb->Bind();

	for (std::vector<FrameBufferAttachmentInfo>::const_iterator it = _RenderBuffersInfo.begin(); it != _RenderBuffersInfo.end(); ++it)
	{
		const FrameBufferAttachmentInfo& Info = (*it);

		if (fb->HasAttachment(Info.m_Attachment))
		{
			LOG(VL_WARN, "FrameBuffer::Create: Framebuffer under construction can not have multiple attachments of type %s");
			continue;
		}

		if (Info.m_GenerateTexture)
		{
			graphics::Texture* tex = graphics::Texture::Create();

			tex->UploadData(Info.m_InternalFormat, Info.m_TextureFormat, Info.m_TextureType, fb->m_Width, fb->m_Height, Info.m_TexturePixels);
			CheckGL();

			tex->Bind();
			// Apply texture parameters.
			for (std::vector<std::pair<GLenum, GLint> >::const_iterator it2 = Info.m_TextureParametersInteger.begin(); it2 != Info.m_TextureParametersInteger.end(); ++it2)
			{
				glTexParameteri(GL_TEXTURE_2D, (*it2).first, (*it2).second);
			}
			for (std::vector<std::pair<GLenum, GLfloat> >::const_iterator it2 = Info.m_TextureParametersFloat.begin(); it2 != Info.m_TextureParametersFloat.end(); ++it2)
			{
				glTexParameterf(GL_TEXTURE_2D, (*it2).first, (*it2).second);
			}
			tex->Unbind();

			// Attach the texture to the FBO by connecting it to the right attachment point.
			glFramebufferTexture2D(GL_FRAMEBUFFER, GLFrameBufferAttachments[Info.m_Attachment], GL_TEXTURE_2D, tex->GetTextureHandle(), 0);
			CheckGL();

			fb->m_Attachments[Info.m_Attachment] = AttachmentPair(ATTACHMENT_STORAGE_TEXTURE, (void*)tex);
		}
		else
		{
			graphics::RenderBuffer* rb = graphics::RenderBuffer::Create(Info.m_InternalFormat, fb->m_Width, fb->m_Height);

			// Attach the generated render buffer to the FBO by connecting it to the right attachment point.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GLFrameBufferAttachments[Info.m_Attachment], GL_RENDERBUFFER, rb->GetBufferHandle());
			CheckGL();

			fb->m_Attachments[Info.m_Attachment] = AttachmentPair(ATTACHMENT_STORAGE_RENDERBUFFER, (void*)rb);
		}
	}

	if (!fb->CheckFrameBufferStatus())
	{
		LOG(VL_ERROR, "FrameBuffer::Create: FrameBuffer construction failed; CheckFrameBufferStatus returned false");
		delete fb;
		return 0;
	}

	fb->Unbind();

	return fb;
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_GLBufferHandle);
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32 FrameBuffer::GetAttachmentTexture( EFrameBufferAttachment _Attachment )
{
#ifdef _DEBUG
	if (!HasShareableTexture(_Attachment))
	{
		LOG(VL_ERROR, "FrameBuffer::GetAttachmenTexture: Attachment point %s does not have a shareable texture because it has been assigned a renderbuffer");
		return 0;
	}
#endif
	return ((Texture*)m_Attachments[_Attachment].second)->GetTextureHandle();
}

bool FrameBuffer::HasAttachment( EFrameBufferAttachment _Attachment )
{
	return (m_Attachments[_Attachment].second != 0);
}

bool FrameBuffer::HasShareableTexture( EFrameBufferAttachment _Attachment )
{
	return (m_Attachments[_Attachment].first == ATTACHMENT_STORAGE_TEXTURE);
}

void FrameBuffer::SetDrawBuffers( EFrameBufferAttachment* _Buffers, uint32 _NumBuffers )
{
	static GLenum buffers[16];		// Because this array is fixed size, there's an artificial maximum of 16 draw buffers
									// to draw to at once. I don't expect to *ever* need to draw to 16 buffers at once, so
									// this should be fine.
	for (uint32 i = 0; i < _NumBuffers; ++i)
	{
		buffers[i] = GLFrameBufferAttachments[_Buffers[i]];
	}
	glDrawBuffers(_NumBuffers, buffers);
}

void FrameBuffer::SetDrawBuffer( EFrameBufferAttachment _Buffer )
{
	glDrawBuffer(GLFrameBufferAttachments[_Buffer]);
}

int32 FrameBuffer::GetWidth() const
{
	return m_Width;
}

int32 FrameBuffer::GetHeight() const
{
	return m_Height;
}

bool FrameBuffer::CheckFrameBufferStatus()
{
	Bind();

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

	Unbind();

	return ret;
}

}

}
