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

	enum EMappingAccess
	{
		BUFFER_MAPPING_READ_ONLY = 0,
		BUFFER_MAPPING_WRITE_ONLY,
		BUFFER_MAPPING_READ_WRITE
	};

	uint32 GetBufferHandle() const;

	void Bind() const;
	void Unbind() const;
	
	//////////////////////////////////////////////////////////////////////////
	// Maps the buffer to client memory and returns a void* to this address.
	// The data can then be accessed as needed. Before OpenGL can access
	// the data again, UnmapBuffer must be called.
	// NOTE: EMappingAccess is, unlike EUsageHint NOT a hint, but actually 
	// constricts the types of access that are allowed on the mapped buffer 
	// (in some OpenGL implementations).
	//////////////////////////////////////////////////////////////////////////
	void* Map( EMappingAccess _Access ) const;

	//////////////////////////////////////////////////////////////////////////
	// Unmaps the buffer from client memory. OpenGL can not use the buffer
	// while it's mapped, so UnmapBuffer must be called whenever
	// OpenGL needs to be able to access it again if it has been mapped.
	// When the buffer is unmapped, it's data store pointer becomes invalid
	// and should no longer be used.
	// Returns true if everything is OK, false if OpenGL detects that the
	// buffer's contents have been corrupted during the mapping process.
	// If this is the case, the buffer data will be re-initialized and
	// the log will show this.
	//////////////////////////////////////////////////////////////////////////
	bool Unmap();

	uint32 GetSize() const;
protected:
	Buffer( EBufferTarget _Target );
	virtual ~Buffer();
	
	// NOTE: When resizing the buffer, simply call this function with the size value for _SizeInBytes and the buffer
	// will be re-allocated.
	// If you want to force a re-allocation of the buffer, you can call this function and pass 0 as value for _Data.
	// Note that a re-allocation will NOT preserve the contents of the buffer. Therefore, after a re-allocation
	// the contents of the buffer are undefined until new data is uploaded.
	// The EUsageHint enumeration value does not limit the functionality 
	// of the buffer, but instead provides hints to the graphics driver so that 
	// it can choose the best memory location for the internal buffer.
	void _UploadData( uint32 _SizeInBytes, const void* _Data, EUsageHint _UsageHint );

	// NOTE: If _OffsetInBytes + _SizeInBytes is greater than the total size of the allocated buffer,
	// an error is generated. It will NOT increase the size of the buffer.
	void _UploadSubData( uint32 _OffsetInBytes, uint32 _SizeInBytes, const void* _Data );
private:
	Buffer();

	uint32 m_GLBufferHandle;
	EBufferTarget m_BufferTarget;
	EUsageHint m_UsageHint;
	uint32 m_SizeInBytes;
};

}

}

#endif