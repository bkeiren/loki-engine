#pragma once

#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "core/intersection/IntersectionResult.h"

namespace loki
{

class Sphere;

class BoundingBox
{
public:
	BoundingBox( const vec3& _Min, const vec3& _Max );
	BoundingBox( f32 _MinX, f32 _MinY, f32 _MinZ, f32 _MaxX, f32 _MaxY, f32 _MaxZ );
	BoundingBox( const vec3* _Vertices, uint32 _NumVertices );
	BoundingBox( f32 _HalfLength );

	const vec3& GetMin() const;
	const vec3& GetMax() const;
	vec3 GetCenter() const;

	IntersectionResult IsInside( const vec3& _P ) const;
	IntersectionResult IsInside( const BoundingBox& _BoundingBox ) const;
	IntersectionResult IsInsideFast( const BoundingBox& _BoundingBox ) const;
	IntersectionResult IsInside( const Sphere& _Sphere ) const;
	IntersectionResult IsInsideFast( const Sphere& _Sphere ) const;
private:
	vec3 m_Min;
	vec3 m_Max;
};

}

#endif