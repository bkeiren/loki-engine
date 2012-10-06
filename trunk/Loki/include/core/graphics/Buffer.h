#pragma once

#ifndef BUFFER_H
#define BUFFER_H

namespace loki
{

namespace graphics
{

class Buffer
{
public:
	enum EBufferTarget
	{
		BUFFER_TARGET_ARRAY_BUFFER = 0,
		BUFFER_TARGET_ELEMENT_ARRAY_BUFFER,
		BUFFER_TARGET_PIXEL_PACK_BUFFER,
		BUFFER_TARGET_PIXEL_UNPACK_BUFFER
	};

	enum EUsageHint
	{
		BUFFER_USAGE_STREAM_DRAW = 0,
		BUFFER_USAGE_STREAM_READ,
		BUFFER_USAGE_STREAM_COPY,

		BUFFER_USAGE_STATIC_DRAW,
		BUFFER_USAGE_STATIC_READ,
		BUFFER_USAGE_STATIC_COPY,

		BUFFER_USAGE_DYNAMIC_DRAW,
		BUFFER_USAGE_DYNAMIC_READ,
		BUFFER_USAGE_DYNAMIC_COPY
	};

	uint32 GetBufferHandle() const;

	void Bind() const;
	void Unbind() const;

protected:
	Buffer( EBufferTarget _Target );
	virtual ~Buffer();
	
	void _UploadData( int32 _Size, void* _Data, EUsageHint _UsageHint );
private:
	Buffer();

	uint32 m_GLBufferHandle;
	EBufferTarget m_BufferTarget;
	EUsageHint m_UsageHint;
};

}

}

#endif