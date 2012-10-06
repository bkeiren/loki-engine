#pragma once

#ifndef INDEXBUFFEROBJECT_H
#define INDEXBUFFEROBJECT_H

#include "core/graphics/Buffer.h"

namespace loki
{

namespace graphics
{

class IndexBuffer	: public Buffer
{
public:
	~IndexBuffer();

	static IndexBuffer* Create( uint32* _Indices, uint32 _NumIndices );

	uint32 GetNumIndices() const;
	const uint32* GetIndicesRAM() const;

private:
	IndexBuffer();

	uint32 m_NumIndices;
	uint32* m_IndicesRAM;
};

}

}

#endif