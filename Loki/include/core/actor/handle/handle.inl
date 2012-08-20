#pragma once

#ifndef HANDLE_INL
#define HANDLE_INL

#include <vector>

namespace loki
{
namespace
{
#define HANDLE_INVALID_INDEX		-1
#define HANDLE_FIRST_VALID_INDEX	0
#define HANDLE_INITIAL_ARRAY_SIZE	8192	// ((8192 * 4) / 1024) = 32 KB of data to store pointers.

	std::vector<LkActor*> Actors = std::vector<LkActor*>(HANDLE_INITIAL_ARRAY_SIZE, NULL);

	//////////////////////////////////////////////////////////////////////////
	// Finds the index for the given pointer if it already exists in
	// the array. If it doesn't HANDLE_INVALID_INDEX is returned.
	//////////////////////////////////////////////////////////////////////////
	int FindIndexForPointer( LkActor* _Pointer )
	{
		// Find the instance.
		for (unsigned int i = HANDLE_FIRST_VALID_INDEX; i < Actors.size(); ++i)
		{
			if (Actors[i] == _Pointer)
			{
				return i;
			}
		}

		return HANDLE_INVALID_INDEX;
	}

	//////////////////////////////////////////////////////////////////////////
	// Searches the array for a free spot.
	//////////////////////////////////////////////////////////////////////////
	int GetFreeHandleIndex()
	{
		// Find the first
		for (unsigned int i = HANDLE_FIRST_VALID_INDEX; i < Actors.size(); ++i)
		{
			if (Actors[i] == NULL)
			{
				return i;
			}
		}

		return HANDLE_INVALID_INDEX;	// -1 is an invalid index.
	}
}

template< typename _T >
LkHandle<_T>::LkHandle( _T* _Instance )	:
	m_Index(HANDLE_INVALID_INDEX)
{
	assert(_Instance != NULL);

	// First find out whether the instance was already stored in the array. If that is the case,
	// we don't want to store it again.
	int idx = FindIndexForPointer(_Instance);
	if (idx != HANDLE_INVALID_INDEX)
	{
		m_Index = idx;
	}
	else
	{
		int idx = GetFreeHandleIndex();
		if (idx != HANDLE_INVALID_INDEX)
		{
			m_Index = idx;					// Store the index in the handle class.
			Actors[m_Index] = _Instance;	// Store the actor instance in the array.
		}
		else
		{
			LOG(VL_ERROR, "Handle::Handle: Unable to locate free storage location for handle! (Array has size of %i)", Actors.size());
		}
	}
}

template< typename _T >
LkHandle<_T>::LkHandle()
{
	LOG(VL_ERROR, "Handle::Handle: Illegal c-tor used");
}

template< typename _T >
LkHandle<_T>::~LkHandle()
{

}

template< typename _T >
bool LkHandle<_T>::IsValid()
{
	return (m_Index != HANDLE_INVALID_INDEX);
}

template< typename _T >
_T* LkHandle<_T>::operator ->()
{
#ifdef _DEBUG
	if (m_Index == HANDLE_INVALID_INDEX)
	{
		LOG(VL_ERROR, "Handle::-> Handle contains invalid index (%i)", m_Index);
	}
#endif
	_T* actor = (_T*)Actors[m_Index];
#ifdef _DEBUG
	if (!actor)
	{
		LOG(VL_ERROR, "Handle::-> Handle indexes to NULL actor (Index %i)", m_Index);
	}
#endif
	return actor;
}

}

#endif