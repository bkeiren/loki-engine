#include "util/typeinfo/typeinfo.h"

namespace loki
{

namespace util
{

TypeInfo::TypeInfo( const std::type_info& _TypeInfo )	:
	m_Ptr(&_TypeInfo)
{

}

TypeInfo::TypeInfo()
{

}

TypeInfo::operator size_t() const
{
	return (size_t)m_Ptr;
}

const char* TypeInfo::GetTypeName() const
{
	return m_Ptr->name();
}

bool operator == ( const TypeInfo& _LHS, const TypeInfo& _RHS ) 
{ 
	return (_LHS.m_Ptr == _RHS.m_Ptr); 
}

bool operator != ( const TypeInfo& _LHS, const TypeInfo& _RHS ) 
{ 
	return (!(_LHS == _RHS)); 
}

bool operator < ( const TypeInfo& _LHS, const TypeInfo& _RHS ) 
{ 
	return (_LHS.m_Ptr < _RHS.m_Ptr); 
}

bool operator > ( const TypeInfo& _LHS, const TypeInfo& _RHS ) 
{ 
	return (_LHS.m_Ptr < _RHS.m_Ptr); 
}

}

}