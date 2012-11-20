#pragma once

#ifndef RECT_H
#define RECT_H

#include "util/property/Property.h"

namespace loki
{

class Rect
{
public:
	Rect( const vec2& _Position, const vec2& _Dimensions );
	Rect( float _X, float _Y, float _Width, float _Height );
	~Rect();

	PROPERTY(vec2, Rect, position,
	{
		return self.m_Position;
	},
	{
		self.m_Position = value;
		self._ComputeDataFromPosition();
	});
	PROPERTY(vec2, Rect, center,
	{
		return self.m_Center;
	},
	{
		self.m_Center = value;
		self._ComputeDataFromCenter();
	});
	PROPERTY(vec2, Rect, dimensions,
	{
		return self.m_Dimensions;
	},
	{
		self.m_Dimensions = value;
		self._ComputeDataFromDimensions();
	});

	bool operator == ( const Rect& _RHS ) const;
	bool operator != ( const Rect& _RHS ) const;
private:
	void _ComputeDataFromPosition();
	void _ComputeDataFromCenter();
	void _ComputeDataFromDimensions();

	vec2 m_Position;
	vec2 m_Center;
	vec2 m_Dimensions;
	vec2 m_TopLeft;
	vec2 m_TopRight;
	vec2 m_BottomLeft;
	vec2 m_BottomRight;
};

}

#endif