#pragma once

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "core/graphics/Enums.h"
#include "glew/glew.h"

namespace loki
{

namespace graphics
{

class FrameBuffer
{
public:
	struct FrameBufferAttachmentInfo
	{
		friend class FrameBuffer;

		FrameBufferAttachmentInfo();
		~FrameBufferAttachmentInfo();

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

		EFrameBufferAttachment m_Attachment;

		bool m_GenerateTexture;	// If true, will generate a texture instead of using a standard OpenGL render buffer.

		// TODO: Provide own enum for _Enum instead of using GL_ defines.
		void AddTextureParameter( GLenum _Enum, GLint _Value );
		void AddTextureParameter( GLenum _Enum, GLfloat _Value );
	private:
		std::vector<std::pair<GLenum, GLint> > m_TextureParametersInteger;
		std::vector<std::pair<GLenum, GLfloat> > m_TextureParametersFloat;
	};

	enum EAttachmentStorageType
	{
		ATTACHMENT_STORAGE_TEXTURE = 0,
		ATTACHMENT_STORAGE_RENDERBUFFER
	};

	typedef std::pair<EAttachmentStorageType, void*>	AttachmentPair;

	~FrameBuffer();

	static FrameBuffer* Create( int32 _Width, int32 _Height, const std::vector<FrameBufferAttachmentInfo>& _RenderBuffersInfo );

	void Bind() const;
	void Unbind() const;

	uint32 GetAttachmentTexture( EFrameBufferAttachment _Attachment );

	bool HasAttachment( EFrameBufferAttachment _Attachment );
	bool HasShareableTexture( EFrameBufferAttachment _Attachment );

	//////////////////////////////////////////////////////////////////////////
	// These two functions assume the frame buffer is bound.
	//////////////////////////////////////////////////////////////////////////
	void SetDrawBuffers( EFrameBufferAttachment* _Buffers, uint32 _NumBuffers );
	void SetDrawBuffer( EFrameBufferAttachment _Buffer );

	int32 GetWidth() const;
	int32 GetHeight() const;

	//////////////////////////////////////////////////////////////////////////
	// Checks the frame buffer completeness status. Returns true if complete,
	// false if something is missing (Check log for precise error).
	//////////////////////////////////////////////////////////////////////////
	bool CheckFrameBufferStatus();
private:
	FrameBuffer();

	uint32 m_GLBufferHandle;
	int32 m_Width;
	int32 m_Height;
	AttachmentPair m_Attachments[_FRAMEBUFFER_ATTACHMENT_COUNT];
};

}

}

#endif