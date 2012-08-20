#pragma once

#ifndef UTIL_INLINE_H
#define UTIL_INLINE_H

#include <list>
#include <Windows.h>

/*
	This file is included in util.h to provide the implementation details of templated functions.
*/

namespace loki
{

template< class T>
static void CleanSTLList( std::list<T>* _List )
{
	_List->remove_if( ClassDeleteAll<T>::DeleteAll );
}

namespace
{

	template< class T >
	class ClassDeleteAll
	{
	public:
		static bool DeleteAll( T _Element )
		{
			DeleteIfPtr(_Element);	// DeleteIfPtr is overloaded to provide two functionalities:
									// If the element in question is a pointer, DeleteIfPtr( T* _Element )
									// is used, otherwise DeleteIfPtr( T& _Element ) is used.
									// This is required to be able to have both pointers and normal objects
									// in the list and still ensure that data is freed correctly (Normal objects
									// are freed automatically, pointers need be freed by using delete.
			return true;
		}

		static void DeleteIfPtr( T* _Element )
		{
			delete T;
		}

		static void DeleteIfPtr( T& _Element )
		{
			// Do nothing, function is provided because this version must exist for the design
			// and implementation of DeleteAll to work.
			return;
		}
	};

}

namespace util
{

inline void Sleep( unsigned int _ms )
{
#ifdef WIN32
	::Sleep(_ms);
#else
#error "util::Sleep has no implementation for non-WIN32 platforms"
#endif
}

}

}

#endif