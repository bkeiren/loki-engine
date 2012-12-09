#pragma once

#ifndef RECT_H
#define RECT_H

#include "util/property/Property.h"

namespace loki
{

class Rect
{
public:
	Rect( const vec2& _Min, const vec2& _Max );
	Rect( f32 _MinX, f32 _MinY, f32 _MaxX, f32 _MaxY );
	~Rect();

	f32 GetMinX() const;
	f32 GetMinY() const;
	f32 GetMaxX() const;
	f32 GetMaxY() const;
	f32 GetWidth() const;
	f32 GetHeight() const;
	const vec2& GetDimensions() const;
	const vec2& GetCenter() const;
	const vec2& GetMin() const;
	const vec2& GetMax() const;
	f32 GetArea() const;

	void Set( const vec2& _Min, const vec2& _Max );
	void Set( f32 _MinX, f32 _MinY, f32 _MaxX, f32 _MaxY );
	void SetMin( const vec2& _Min );
	void SetMin( f32 _MinX, f32 _MinY );
	void SetMax( const vec2& _Max );
	void SetMax( f32 _MaxX, f32 _MaxY );

	bool operator == ( const Rect& _RHS ) const;
	bool operator != ( const Rect& _RHS ) const;
private:
	void _ComputeDataFromMinMax();

	vec2 m_Center;
	vec2 m_Dimensions;
	vec2 m_Min;
	vec2 m_Max;
};

}

#endif