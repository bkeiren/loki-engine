#pragma once

#ifndef PROPERTY_H
#define PROPERTY_H

#include <stddef.h>

namespace loki
{

namespace util
{

// Public property.
#define PROPERTY(T, Class, name, get, set)  private:																		\
											static size_t _Offset_ ## name () { return offsetof (Class, name); }			\
											static T const &_Get_ ## name (Class /*const*/& self) get						\
											static void _Set_ ## name (Class& self, T const& value) set						\
											public:																			\
											loki::util::Property<Class, T, _Get_ ## name, _Set_ ## name, _Offset_ ## name> name

// Custom property scope (Public, private, protected).
#define PROPERTY_SCOPED(T, Class, name, accessscope, get, set)	private:																		\
																static size_t _Offset_ ## name () { return offsetof (Class, name); }			\
																static T const &_Get_ ## name (Class /*const*/& self) get						\
																static void _Set_ ## name (Class& self, T const& value) set						\
																accessscope:																	\
																loki::util::Property<Class, T, _Get_ ## name, _Set_ ## name, _Offset_ ## name> name

//////////////////////////////////////////////////////////////////////////
// Implemented using http://xinutec.org/~pippijn/home/programming/cpp/properties
//////////////////////////////////////////////////////////////////////////
#define TEMPLATELINE			<	typename _Class,							\
									typename _T,								\
									_T const & (_Get) (_Class /*const*/ &),		\
									void (_Set) (_Class &, _T const &),			\
									size_t (_Offset) ()	>
#define TEMPLATEARGUMENTLINE	< _Class, _T, _Get, _Set, _Offset >
template TEMPLATELINE
class Property
{
	friend _Class;
public:
	Property();
	~Property();

	Property TEMPLATEARGUMENTLINE& operator = (_T const& rhs);
	Property TEMPLATEARGUMENTLINE& operator += (_T const& rhs);
	Property TEMPLATEARGUMENTLINE& operator -= (_T const& rhs);
	Property TEMPLATEARGUMENTLINE& operator *= (_T const& rhs);
	Property TEMPLATEARGUMENTLINE& operator /= (_T const& rhs);

	operator _T const& () const;

	_T* operator -> ();

	_T const* operator -> () const;
private:
	_Class& Self ();

	_Class const& Self () const;
};

#include "util/property/Property.inl"

#undef TEMPLATEARGUMENTLINE
#undef TEMPLATELINE

}

}

#endif
