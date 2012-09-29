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

	static IndexBufferObject* Create( unsigned int* _Indices, unsigned int _NumIndices );

	unsigned int GetNumIndices() const;
	unsigned int GetBufferHandle() const;
	const unsigned int* GetIndicesRAM() const;
private:
	IndexBufferObject();

	unsigned int m_NumIndices;
	unsigned int m_GLBufferHandle;
	unsigned int* m_IndicesRAM;
};

}

}

#endif