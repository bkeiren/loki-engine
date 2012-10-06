#include "core/graphics/PixelBuffer.h"

namespace loki
{

namespace graphics
{

PixelBuffer::PixelBuffer( EPixelBufferTarget _Target )	:
	Buffer( (_Target == PIXEL_BUFFER_PACK__GPU_TO_CPU) ? (Buffer::BUFFER_TARGET_PIXEL_PACK_BUFFER) : (Buffer::BUFFER_TARGET_PIXEL_UNPACK_BUFFER) )
{
	Resize(1);
}

PixelBuffer::PixelBuffer()	:
	Buffer(Buffer::BUFFER_TARGET_ARRAY_BUFFER)
{
	ILLEGAL_CTOR_ERROR("PixelBuffer");
}

PixelBuffer::~PixelBuffer()
{

}

PixelBuffer* PixelBuffer::Create( EPixelBufferTarget _Target )
{
	PixelBuffer* pbo = new PixelBuffer(_Target);

	return pbo;
}

}

}