#pragma once

#ifndef PIXELBUFFER_H
#define PIXELBUFFER_H

#include "core/graphics/Buffer.h"

namespace loki
{

namespace graphics
{

class PixelBuffer	: public Buffer
{
public:
	enum EPixelBufferTarget
	{
		PIXEL_BUFFER_PACK__GPU_TO_CPU = 0,
		PIXEL_BUFFER_UNPACK__CPU_TO_GPU,
	};

	~PixelBuffer();

	static PixelBuffer* Create( EPixelBufferTarget _Target );
private:
	PixelBuffer( EPixelBufferTarget _Target );
	PixelBuffer();
};

}

}

#endif