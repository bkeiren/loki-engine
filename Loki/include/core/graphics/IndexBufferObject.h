#pragma once

#ifndef INDEXBUFFEROBJECT_H
#define INDEXBUFFEROBJECT_H

namespace loki
{

namespace graphics
{

class IndexBufferObject
{
public:
	~IndexBufferObject();

	static IndexBufferObject* Create( uint32* _Indices, uint32 _NumIndices );

	uint32 GetNumIndices() const;
	uint32 GetBufferHandle() const;
	const uint32* GetIndicesRAM() const;
private:
	IndexBufferObject();

	uint32 m_NumIndices;
	uint32 m_GLBufferHandle;
	uint32* m_IndicesRAM;
};

}

}

#endif