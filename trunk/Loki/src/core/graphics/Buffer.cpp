#include "core/graphics/Buffer.h"
#include "glew/glew.h"

namespace loki
{

namespace graphics
{

namespace
{

inline GLenum GetOpenGLTargetEnum( Buffer::EBufferTarget _BufferTarget )
{
	switch (_BufferTarget)
	{
	case Buffer::BUFFER_TARGET_ARRAY_BUFFER:
		{
			return GL_ARRAY_BUFFER;
			break;
		}
	case Buffer::BUFFER_TARGET_ELEMENT_ARRAY_BUFFER:
		{
			return GL_ELEMENT_ARRAY_BUFFER;
			break;
		}
	case Buffer::BUFFER_TARGET_PIXEL_PACK_BUFFER:
		{
			return GL_PIXEL_PACK_BUFFER;
			break;
		}
	case Buffer::BUFFER_TARGET_PIXEL_UNPACK_BUFFER:
		{
			return GL_PIXEL_UNPACK_BUFFER;
			break;
		}
	}
	return 0;
}

inline GLenum GetOpenGLUsageHintEnum( Buffer::EUsageHint _UsageHint )
{
	switch (_UsageHint)
	{
	case Buffer::BUFFER_USAGE_STREAM_DRAW:
		{
			return GL_STREAM_DRAW;
			break;
		}
	case Buffer::BUFFER_USAGE_STREAM_READ:
		{
			return GL_STREAM_READ;
			break;
		}
	case Buffer::BUFFER_USAGE_STREAM_COPY:
		{
			return GL_STREAM_COPY;
			break;
		}
	case Buffer::BUFFER_USAGE_STATIC_DRAW:
		{
			return GL_STATIC_DRAW;
			break;
		}
	case Buffer::BUFFER_USAGE_STATIC_READ:
		{
			return GL_STATIC_READ;
			break;
		}
	case Buffer::BUFFER_USAGE_STATIC_COPY:
		{
			return GL_STATIC_COPY;
			break;
		}
	case Buffer::BUFFER_USAGE_DYNAMIC_DRAW:
		{
			return GL_DYNAMIC_DRAW;
			break;
		}
	case Buffer::BUFFER_USAGE_DYNAMIC_READ:
		{
			return GL_DYNAMIC_READ;
			break;
		}
	case Buffer::BUFFER_USAGE_DYNAMIC_COPY:
		{
			return GL_DYNAMIC_COPY;
			break;
		}
	}
	return 0;
}

}

Buffer::Buffer( EBufferTarget _Target )	:
	m_BufferTarget(_Target),
	m_GLBufferHandle(0),
	m_UsageHint(BUFFER_USAGE_STATIC_DRAW)
{
	glGenBuffers(1, &m_GLBufferHandle);
}

Buffer::Buffer()
{
	ILLEGAL_CTOR_ERROR("Buffer");
}

Buffer::~Buffer()
{
	if (m_GLBufferHandle != 0)
	{
		glDeleteBuffers(1, &m_GLBufferHandle);
		m_GLBufferHandle = 0;
	}
}

uint32 Buffer::GetBufferHandle() const
{
	return m_GLBufferHandle;
}

void Buffer::Bind() const
{
	glBindBuffer(GetOpenGLTargetEnum(m_BufferTarget), m_GLBufferHandle);
}

void Buffer::Unbind() const
{
	glBindBuffer(GetOpenGLTargetEnum(m_BufferTarget), 0);
}

void Buffer::_UploadData( int32 _Size, void* _Data, EUsageHint _UsageHint )
{
	m_UsageHint = _UsageHint;

	Bind();
	glBufferData(GetOpenGLTargetEnum(m_BufferTarget), _Size, _Data, GetOpenGLUsageHintEnum(m_UsageHint));
	Unbind();
}

}

}