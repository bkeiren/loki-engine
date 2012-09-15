#include "core/renderer/pixelbufferobject.h"

namespace loki
{

namespace renderer
{

LkPixelBufferObject::LkPixelBufferObject( EPBO_USAGE_HINT_DIRECTION _UsageHintDirection, EPBO_USAGE_HINT_PATTERN _UsageHintPattern, int _SizeInBytes, const void* _InitialData /*= 0*/ )	:
	m_Buffer(0),
	m_UsageHintDirection(_UsageHintDirection),
	m_UsageHintPattern(_UsageHintPattern),
	m_SizeInBytes(_SizeInBytes)
{
	assert(m_SizeInBytes > 0);

	glGenBuffers(1, &m_Buffer);

	if (!m_Buffer)
	{
		LOG(VL_ERROR, "PixelBufferObject::PixelBufferObject: Failed to create buffer");
	}

	BufferData(_InitialData);
	Unbind();
}

LkPixelBufferObject::LkPixelBufferObject()
{
	ILLEGAL_CTOR_ERROR("PixelBufferObject");
}

LkPixelBufferObject::~LkPixelBufferObject()
{
	// NOTE: When deleting a buffer, it will automatically be unmapped if required. No unmapping is needed.

	glDeleteBuffers(1, &m_Buffer);
	m_Buffer = 0;
}

void LkPixelBufferObject::Bind() const
{
	glBindBuffer(m_UsageHintDirection, m_Buffer);
}

void LkPixelBufferObject::Unbind() const
{
	glBindBuffer(m_UsageHintDirection, 0);
}

void* LkPixelBufferObject::MapBuffer( EPBO_MAP_ACCESS _MapAccess ) const
{
	Bind();
	return glMapBuffer(m_UsageHintDirection, _MapAccess);
}

bool LkPixelBufferObject::UnmapBuffer() const
{
	bool b = (bool)glUnmapBuffer(m_UsageHintDirection);
	if (!b)
	{
		LOG(VL_WARN, "PixelBufferObject::UnmapBuffer: glUnmapBuffer returned false, the buffer contents were corrupted while the buffer was mapped. Buffer will be re-initialized");
		BufferData(0);
	}
	return b;
}

void LkPixelBufferObject::BufferSubData( int _OffsetInBytes, int _SizeInBytes, const void* _Data ) const
{
	Bind();
	glBufferSubData(m_UsageHintDirection, _OffsetInBytes, _SizeInBytes, _Data);
}

void LkPixelBufferObject::BufferData( const void* _Data ) const
{
	Bind();
	glBufferData(m_UsageHintDirection, m_SizeInBytes, _Data, m_UsageHintPattern);
}

void LkPixelBufferObject::Resize( int _SizeInBytes )
{
	assert(_SizeInBytes > 0);
	m_SizeInBytes = _SizeInBytes;

	BufferData(0);
	Unbind();
}

int LkPixelBufferObject::GetSize() const
{
	return m_SizeInBytes;
}

EPBO_USAGE_HINT_DIRECTION LkPixelBufferObject::GetUsageHintDirection() const
{
	return m_UsageHintDirection;
}

EPBO_USAGE_HINT_PATTERN LkPixelBufferObject::GetUsageHintPattern() const
{
	return m_UsageHintPattern;
}

}

}