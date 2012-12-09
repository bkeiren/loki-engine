#include "core/rect/Rect.h"

namespace loki
{

Rect::Rect( const vec2& _Min, const vec2& _Max )	:
	m_Min(_Min)
	,m_Max(_Max)
{
	_ComputeDataFromMinMax();
}

Rect::Rect( f32 _MinX, f32 _MinY, f32 _MaxX, f32 _MaxY )	:
	m_Min(vec2(_MinX, _MinY))
	,m_Max(vec2(_MaxX, _MaxY))
{
	_ComputeDataFromMinMax();
}

Rect::~Rect()
{
	
}

f32 Rect::GetMinX() const
{
	return m_Min.x;
}

f32 Rect::GetMinY() const
{
	return m_Min.y;
}

f32 Rect::GetMaxX() const
{
	return m_Max.x;
}

f32 Rect::GetMaxY() const
{
	return m_Max.y;
}

f32 Rect::GetWidth() const
{
	return m_Dimensions.x;
}

f32 Rect::GetHeight() const
{
	return m_Dimensions.y;
}

const vec2& Rect::GetDimensions() const
{
	return m_Dimensions;
}

const vec2& Rect::GetCenter() const
{
	return m_Center;
}

const vec2& Rect::GetMin() const
{
	return m_Min;
}

const vec2& Rect::GetMax() const
{
	return m_Max;
}

f32 Rect::GetArea() const
{
	return m_Dimensions.x * m_Dimensions.y;
}

void Rect::Set( const vec2& _Min, const vec2& _Max )
{
	m_Min = _Min;
	m_Max = _Max;
	_ComputeDataFromMinMax();
}

void Rect::Set( f32 _MinX, f32 _MinY, f32 _MaxX, f32 _MaxY )
{
	m_Min = vec2(_MinX, _MinY);
	m_Max = vec2(_MaxX, _MaxY);
	_ComputeDataFromMinMax();
}

void Rect::SetMin( const vec2& _Min )
{
	m_Min = _Min;
	_ComputeDataFromMinMax();
}

void Rect::SetMin( f32 _MinX, f32 _MinY )
{
	m_Min = vec2(_MinX, _MinY);
	_ComputeDataFromMinMax();
}

void Rect::SetMax( const vec2& _Max )
{
	m_Max = _Max;
	_ComputeDataFromMinMax();
}

void Rect::SetMax( f32 _MaxX, f32 _MaxY )
{
	m_Max = vec2(_MaxX, _MaxY);
	_ComputeDataFromMinMax();
}

bool Rect::operator == ( const Rect& _RHS ) const
{
	return ( (m_Min.x == _RHS.m_Min.x) && 
			 (m_Min.y == _RHS.m_Min.y) &&
			 (m_Max.x == _RHS.m_Max.x) && 
			 (m_Max.y == _RHS.m_Max.y) );
}

bool Rect::operator != ( const Rect& _RHS ) const
{
	return !(this->operator == (_RHS));
}

void Rect::_ComputeDataFromMinMax()
{
	m_Dimensions = vec2(m_Max - m_Min);
	m_Center = m_Min + (m_Dimensions * 0.5f);
}

}