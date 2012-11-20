#include "core/rect/Rect.h"

namespace loki
{

Rect::Rect( const vec2& _Position, const vec2& _Dimensions )	:
	m_Position(_Position),
	m_Dimensions(_Dimensions),
	m_Center(m_Position + (m_Dimensions * 0.5f))
{
	_ComputeDataFromPosition();
}

Rect::Rect( float _X, float _Y, float _Width, float _Height )	:
	m_Position(vec2(_X, _Y)),
	m_Dimensions(vec2(_Width, _Height)),
	m_Center(m_Position + (m_Dimensions * 0.5f))
{
	_ComputeDataFromPosition();
}

Rect::~Rect()
{

}

bool Rect::operator == ( const Rect& _RHS ) const
{
	return ( (m_Position.x == _RHS.m_Position.x) && 
			 (m_Position.y == _RHS.m_Position.y) &&
			 (m_Dimensions.x == _RHS.m_Dimensions.x) && 
			 (m_Dimensions.y == _RHS.m_Dimensions.y) );
}

bool Rect::operator != ( const Rect& _RHS ) const
{
	return !(this->operator == (_RHS));
}

void Rect::_ComputeDataFromPosition()
{
	m_TopLeft = m_Position;
	m_TopRight = m_Position + vec2(m_Dimensions.x, 0.0f);
	m_BottomLeft = m_Position + vec2(0.0f, m_Dimensions.y);
	m_BottomRight = m_Position + m_Dimensions;
	m_Center = m_Position + (m_Dimensions * 0.5f);
}

void Rect::_ComputeDataFromCenter()
{
	m_Position = m_Center - vec2(m_Dimensions * 0.5f);
	m_TopLeft = m_Position;
	m_TopRight = m_Position + vec2(m_Dimensions.x, 0.0f);
	m_BottomLeft = m_Position + vec2(0.0f, m_Dimensions.y);
	m_BottomRight = m_Position + m_Dimensions;
}

void Rect::_ComputeDataFromDimensions()
{
	m_TopRight = m_Position + vec2(m_Dimensions.x, 0.0f);
	m_BottomLeft = m_Position + vec2(0.0f, m_Dimensions.y);
	m_BottomRight = m_Position + m_Dimensions;
	m_Center = m_Position + (m_Dimensions * 0.5f);
}

}