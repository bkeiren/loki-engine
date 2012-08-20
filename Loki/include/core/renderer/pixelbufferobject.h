#pragma once

#ifndef PIXELBUFFEROBJECT_H
#define PIXELBUFFEROBJECT_H

#include <GLEW//glew.h>

namespace loki
{

namespace renderer
{

enum EPBO_USAGE_HINT_DIRECTION
{
	PBO_PACK = GL_PIXEL_PACK_BUFFER,	// Used for PBO's which need to transfer pixel data from OpenGL to the application (GPU -> CPU)
	PBO_GPU_TO_CPU = PBO_PACK,

	PBO_UNPACK = GL_PIXEL_UNPACK_BUFFER,		// Used for PBO's which need to transfer pixel data from the application to OpenGL (CPU -> GPU)
	PBO_CPU_TO_GPU = PBO_UNPACK
};

enum EPBO_USAGE_HINT_PATTERN
{
	//////////////////////////////////////////////////////////////////////////
	// STREAM:	The data store contents will be modified once and used at most a few times.
	// STATIC:	The data store contents will be modified once and used many times.
	// DYNAMIC: The data store contents will be modified repeatedly and used many times.
	//
	// DRAW:	The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
	// READ:	The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
	// COPY:	The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.
	//////////////////////////////////////////////////////////////////////////

	PBO_STREAM_DRAW = GL_STREAM_DRAW,
	PBO_STREAM_READ = GL_STREAM_READ,
	PBO_STREAM_COPY = GL_STREAM_COPY,
	PBO_STATIC_DRAW = GL_STATIC_DRAW,
	PBO_STATIC_READ = GL_STATIC_READ,
	PBO_STATIC_COPY = GL_STATIC_COPY,
	PBO_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
	PBO_DYNAMIC_READ = GL_DYNAMIC_READ,
	PBO_DYNAMIC_COPY = GL_DYNAMIC_COPY
};

enum EPBO_MAP_ACCESS
{
	PBO_MAP_READ_ONLY = GL_READ_ONLY,
	PBO_MAP_WRITE_ONLY = GL_WRITE_ONLY,
	PBO_MAP_READ_WRITE = GL_READ_WRITE
};

class LkPixelBufferObject
{
public:
	//////////////////////////////////////////////////////////////////////////
	// _UsageHint* indicates how the pixel buffer object is supposed to be
	// used. This does not limit the functionality but instead
	// it provides mere hints to the graphics driver so that it can
	// choose the best memory location for the internal buffer.
	//////////////////////////////////////////////////////////////////////////
	LkPixelBufferObject( EPBO_USAGE_HINT_DIRECTION _UsageHintDirection, EPBO_USAGE_HINT_PATTERN _UsageHintPattern, int _SizeInBytes, const void* _InitialData = 0 );
	~LkPixelBufferObject();

	void Bind() const;
	void Unbind() const;

	//////////////////////////////////////////////////////////////////////////
	// Maps the buffer to client memory and returns a void* to this address.
	// The data can then be accessed as needed. Before OpenGL can access
	// the data again, UnmapBuffer must be called.
	// NOTE: _MapAccess is, unlike _UsageHintDirection and _UsageHintPattern
	// NOT a hint, but actually constricts the types of access that are 
	// allowed on the mapped buffer (in some OpenGL implementations).	
	//////////////////////////////////////////////////////////////////////////
	void* MapBuffer( EPBO_MAP_ACCESS _MapAccess ) const;

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
	bool UnmapBuffer() const;

	//////////////////////////////////////////////////////////////////////////
	// Binds the buffer and sets data to be stored in it.
	//////////////////////////////////////////////////////////////////////////
	void BufferSubData( int _OffsetInBytes, int _SizeInBytes, const void* _Data ) const;

	void BufferData( const void* _Data ) const;

	//////////////////////////////////////////////////////////////////////////
	// Resizes the buffer.
	// This re-initializes the buffer and therefore the data that is present
	// in the buffer after resizing is undefined.
	//////////////////////////////////////////////////////////////////////////
	void Resize( int _SizeInBytes );

	//////////////////////////////////////////////////////////////////////////
	// Returns the size in bytes of the buffer.
	//////////////////////////////////////////////////////////////////////////
	int GetSize() const;

	EPBO_USAGE_HINT_DIRECTION GetUsageHintDirection() const;
	EPBO_USAGE_HINT_PATTERN GetUsageHintPattern() const;
private:
	LkPixelBufferObject();

	//////////////////////////////////////////////////////////////////////////
	// OpenGL buffer ID.
	//////////////////////////////////////////////////////////////////////////
	unsigned int m_Buffer;

	//////////////////////////////////////////////////////////////////////////
	// The usage hints that were passed to the constructor.
	//////////////////////////////////////////////////////////////////////////
	EPBO_USAGE_HINT_DIRECTION m_UsageHintDirection;
	EPBO_USAGE_HINT_PATTERN m_UsageHintPattern;

	int m_SizeInBytes;
};

}

}

#endif