#pragma once

#ifndef PROPERTY_H
#define PROPERTY_H

namespace loki
{

namespace util
{

//////////////////////////////////////////////////////////////////////////
// Implemented using code from http://stackoverflow.com/questions/3742740/how-to-define-or-implement-c-sharp-property-in-iso-c
//////////////////////////////////////////////////////////////////////////
#define TEMPLATELINE	< class _ParentClassType, typename _PropertyType >
#define TEMPLATEARGUMENTLINE	< _ParentClassType, _PropertyType >
template TEMPLATELINE
class Property
{
	friend _ParentClassType;
public:
	typedef void (_ParentClassType::*SetFunctor)( const _PropertyType& _Value);
	typedef const _PropertyType& (_ParentClassType::*GetFunctor)();

	Property();
	~Property();

	void Init(_ParentClassType* _Class, GetFunctor _GetFunctor, SetFunctor _SetFunctor );

	inline operator _PropertyType(void);
	inline const _PropertyType& operator = (const _PropertyType& _Value);
	inline const _PropertyType& operator += (const _PropertyType& _Value);
	inline const _PropertyType& operator -= (const _PropertyType& _Value);
	inline const _PropertyType& operator *= (const _PropertyType& _Value);
	inline const _PropertyType& operator /= (const _PropertyType& _Value);
private:
	_PropertyType m_Value;
	SetFunctor m_SetFunctor;
	GetFunctor m_GetFunctor;
	_ParentClassType* m_Class;

	inline const _PropertyType& Get(void);
	inline void Set(const _PropertyType& _Value);
};

#include "util/property/Property.inl"

#undef TEMPLATEARGUMENTLINE
#undef TEMPLATELINE

}

}

#endif
