#include "core/graphics/DisplayList.h"
#include "glew/glew.h"

namespace loki
{

namespace graphics
{

DisplayList::DisplayList()
{
	ILLEGAL_CTOR_ERROR("DisplayList")
}

DisplayList::DisplayList( int32 _NumLists )	:	
	m_NumLists(_NumLists),
	m_Lists(0),
	m_Compiled(false)
{
	assert(m_NumLists > 0);

	m_Lists = new uint32[m_NumLists];

	m_Lists[0] = glGenLists(m_NumLists);

	if (m_Lists[0] == 0)
	{
		LOG(VL_ERROR, "DisplayList::DisplayList: Failed to obtain display list ID from API: API returned 0 on glGenLists");
		return;
	}

	for (int32 i = 1; i < m_NumLists; ++i)
	{
		m_Lists[i] = m_Lists[0] + i; 
	}
}

DisplayList::~DisplayList()
{
	if (m_Lists != 0)
	{
		glDeleteLists(m_Lists[0], m_NumLists);
	}

	delete[] m_Lists;
}

DisplayList* DisplayList::Create( int32 _NumLists /*= 1*/ )
{
	if (_NumLists <= 0)
	{
		LOG(VL_ERROR, "DisplayList::Create: A display list must consist of at least 1 list");
		return 0;
	}

	return new DisplayList(_NumLists);
}

void DisplayList::BeginList()
{
	glNewList(m_Lists[0], GL_COMPILE);
	m_Compiled = false;
}

void DisplayList::BeginListIndexed( int32 _Index )
{
	assert(_Index >= 0 && _Index < m_NumLists);
	glNewList(m_Lists[_Index], GL_COMPILE);
	m_Compiled = false;
}

void DisplayList::EndList()
{
	glEndList();
	m_Compiled = true;
}

void DisplayList::Draw() const
{
#ifdef _DEBUG
	if (!IsCompiled())
	{
		LOG(VL_WARN, "DisplayList::Draw: DisplayList has not been compiled yet");
	}
#endif
	glCallLists(m_NumLists, GL_UNSIGNED_INT, (void*)m_Lists);
}

void DisplayList::DrawIndexed( int32 _Index ) const
{
	assert(_Index >= 0 && _Index < m_NumLists);

#ifdef _DEBUG
	if (!IsCompiled())
	{
		LOG(VL_ERROR, "DisplayList::Draw: DisplayList has not been compiled yet");
	}
#endif

	glCallList(m_Lists[_Index]);
}

bool DisplayList::IsCompiled() const
{
	return m_Compiled;
}

}

}