#pragma once

#ifndef DISPLAYLIST_H
#define DISPLAYLIST_H

namespace loki
{

namespace graphics
{

class DisplayList
{
public:
	~DisplayList();

	static DisplayList* Create( int32 _NumLists = 1 );

	// Call this before making draw commands.
	// When this object has created multiple lists, calling BeginList is the same as 
	// calling BeginListIndexed with 0 as argument.
	void BeginList();

	// Call this when this object has created multiple display lists in order to start the list at index _Index.
	// Calling BeginListIndexed with _Index = 0 is the same as calling BeginList.
	void BeginListIndexed( int32 _Index );

	// Call this when you're done making draw commands. This will compile the display list.
	void EndList();

	// When multiple lists were created by this object, this function calls all lists.
	// If only one list was created, calls only that list.
	void Draw() const;

	// When multiple lists were created by this object, this function calls the list with index _Index.
	void DrawIndexed( int32 _Index ) const;

	bool IsCompiled() const;
private:
	DisplayList();
	DisplayList( int32 _NumLists );

	uint32* m_Lists;
	int32 m_NumLists;
	bool m_Compiled;
};

}

}

#endif