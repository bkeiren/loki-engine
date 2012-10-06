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

inline GLenum GetOpenGLMappingAccessEnum( Buffer::EMappingAccess _Access )
{
	switch (_Access)
	{
	case Buffer::BUFFER_MAPPING_READ_ONLY:
		{
			return GL_READ_ONLY;
			break;
		}
	case Buffer::BUFFER_MAPPING_WRITE_ONLY:
		{
			return GL_WRITE_ONLY;
			break;
		}
	case Buffer::BUFFER_MAPPING_READ_WRITE:
		{
			return GL_READ_WRITE;
			break;
		}
	}
	return 0;
}

#define BUFFER_TARGET	GetOpenGLTargetEnum(m_BufferTarget)

}

Buffer::Buffer( EBufferTarget _Target )	:
	m_BufferTarget(_Target),
	m_GLBufferHandle(0),
	m_UsageHint(BUFFER_USAGE_STATIC_DRAW),
	m_SizeInBytes(0)
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
	glBindBuffer(BUFFER_TARGET, m_GLBufferHandle);
}

void Buffer::Unbind() const
{
	glBindBuffer(BUFFER_TARGET, 0);
}

void* Buffer::Map( EMappingAccess _Access ) const
{
	Bind();
	
	void* p = glMapBuffer(BUFFER_TARGET, GetOpenGLMappingAccessEnum(_Access));

	if (!p)
	{
		LOG(VL_WARN, "Buffer::Map: glMapBuffer returned a zero-address. Buffer data store was not mapped.");
	}
	return p;
}

bool Buffer::Unmap()
{
	bool b = (bool)glUnmapBuffer(BUFFER_TARGET);
	if (!b)
	{
		LOG(VL_WARN, "Buffer::Unmap: glUnmapBuffer returned false. Buffer contents were corrupted while buffer was mapped; buffer will be re-initialized");
		UploadData(m_SizeInBytes, 0, m_UsageHint);
	}
	return b;
}

uint32 Buffer::GetSize() const
{
	return m_SizeInBytes;
}

void Buffer::Resize( uint32 _SizeInBytes )
{
	UploadData(_SizeInBytes, 0, m_UsageHint);
}

void Buffer::UploadData( uint32 _SizeInBytes, const void* _Data, EUsageHint _UsageHint )
{
	Bind();

	// Optimization: If we're replacing the entire data store (and not trying to initialize it by passing 0 for _Data),
	// we use UploadSubData instead because it is faster than reinitializing the entire data store.
	// Although we can expect client code to take this into account and use UploadSubData directly when it's better,
	// we still do this check because it simplifies things a little (And we don't have to worry about unexpected 
	// re-allocations of the data store).
	if (_SizeInBytes != m_SizeInBytes || _Data == 0)
	{
		glBufferData(BUFFER_TARGET, _SizeInBytes, _Data, GetOpenGLUsageHintEnum(_UsageHint));
	}
	else
	{
		UploadSubData(0, _SizeInBytes, _Data);
	}

	Unbind();

	m_UsageHint = _UsageHint;
	m_SizeInBytes = _SizeInBytes;
}

void Buffer::UploadSubData( uint32 _OffsetInBytes, uint32 _SizeInBytes, const void* _Data )
{
	Bind();
	glBufferSubData(BUFFER_TARGET, _OffsetInBytes, _SizeInBytes, _Data);
	Unbind();
}

}

}