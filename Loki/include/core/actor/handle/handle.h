#pragma once

#ifndef HANDLE_H
#define HANDLE_H

namespace loki
{

//////////////////////////////////////////////////////////////////////////
// This handle class is designed to be templatable to types that inherit
// from the base Actor class.
// A handle to an Actor instance can be obtained in these ways:
// Pawn* pawn = ...;	// Pawn instance.
// Handle<Pawn> handle = Handle<Pawn>(pawn);	// Create a handle for the
//												// instance.
// handle->...	// Access the pawn instance through the overloaded ->
//				// operator. NOTE: If you're using Visual Assist, note that
//				// it might not recognize the fact that -> is overloaded,
//				// and will therefore not give you a list of functionality
//				// for the returned type. You CAN still access it as if you
//				// were accessing the raw pointer to the type you're using.
//
// NOTE: Classes that do not inherit from Actor can NOT be instantiated.
//////////////////////////////////////////////////////////////////////////
template< typename _T >
class LkHandle
{
public:
	LkHandle( _T* _Instance );
	~LkHandle();

	bool IsValid();

	_T* operator ->();
private:
	LkHandle();	// Private default c-tor.

	// Index into an array of actors.
	int m_Index;
};

}

#include "core/actor/handle/handle.inl"

#endif