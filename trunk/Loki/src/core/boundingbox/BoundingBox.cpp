#include "core/boundingbox/BoundingBox.h"
#include "core/sphere/Sphere.h"
namespace loki
{

BoundingBox::BoundingBox( const vec3& _Min, const vec3& _Max )	:
	 m_Min(_Min)
	,m_Max(_Max)
{

}

BoundingBox::BoundingBox( f32 _MinX, f32 _MinY, f32 _MinZ, f32 _MaxX, f32 _MaxY, f32 _MaxZ )	:
	 m_Min(vec3(_MinX, _MinY, _MinZ))
	,m_Max(vec3(_MaxX, _MaxY, _MaxZ))
{
	
}

BoundingBox::BoundingBox( const vec3* _Vertices, uint32 _NumVertices )
{
	vec3 min = vec3(0.0f, 0.0f, 0.0f);
	vec3 max = vec3(0.0f, 0.0f, 0.0f);
	for (uint32 i = 0; i < _NumVertices; ++i)
	{
		const vec3& v = _Vertices[i];

#define HELPER(a)		\
	if (v.a < min.a)	\
	{					\
		min.a = v.a;	\
	}					\
	if (v.a > max.a)	\
	{					\
		max.a = v.a;	\
	}

		HELPER(x)
		HELPER(y)
		HELPER(z)

#undef HELPER	
	}

	m_Min = min;
	m_Max = max;
}

BoundingBox::BoundingBox( f32 _HalfLength )	:
	 m_Min(-_HalfLength, -_HalfLength, -_HalfLength)
	,m_Max(_HalfLength, _HalfLength, _HalfLength)
{

}

const vec3& BoundingBox::GetMin() const
{
	return m_Min;
}

const vec3& BoundingBox::GetMax() const
{
	return m_Max;
}

vec3 BoundingBox::GetCenter() const
{
	return m_Min + ((m_Max - m_Min) * 0.5f);
}

IntersectionResult BoundingBox::IsInside( const vec3& _P ) const
{
	bool inside = (_P.x >= m_Min.x && _P.x <= m_Max.x && 
				   _P.y >= m_Min.y && _P.y <= m_Max.y && 
				   _P.z >= m_Min.z && _P.z <= m_Max.z);
	return IntersectionResult(inside ? IntersectionResult::INSIDE : IntersectionResult::OUTSIDE);
}

IntersectionResult BoundingBox::IsInside( const BoundingBox& _BoundingBox ) const
{
	if (_BoundingBox.GetMax().x < GetMin().x || _BoundingBox.GetMin().x > GetMax().x || 
		_BoundingBox.GetMax().y < GetMin().y || _BoundingBox.GetMin().y > GetMax().y ||
		_BoundingBox.GetMax().z < GetMin().z || _BoundingBox.GetMin().z > GetMax().z)
	{
		return IntersectionResult(IntersectionResult::OUTSIDE);
	}
	else if (_BoundingBox.GetMin().x < GetMin().x || _BoundingBox.GetMax().x > GetMax().x || 
			 _BoundingBox.GetMin().y < GetMin().y || _BoundingBox.GetMax().y > GetMax().y ||
			 _BoundingBox.GetMin().z < GetMin().z || _BoundingBox.GetMax().z > GetMax().z)
	{
		return IntersectionResult(IntersectionResult::INTERSECTS);
	}
	return IntersectionResult(IntersectionResult::INSIDE);
}

IntersectionResult BoundingBox::IsInsideFast( const BoundingBox& _BoundingBox ) const
{
	if (_BoundingBox.GetMax().x < GetMin().x || _BoundingBox.GetMin().x > GetMax().x || 
		_BoundingBox.GetMax().y < GetMin().y || _BoundingBox.GetMin().y > GetMax().y ||
		_BoundingBox.GetMax().z < GetMin().z || _BoundingBox.GetMin().z > GetMax().z)
	{
		return IntersectionResult(IntersectionResult::OUTSIDE);
	}
	return IntersectionResult(IntersectionResult::INSIDE);
}

IntersectionResult BoundingBox::IsInside( const Sphere& _Sphere ) const
{
	f32 t;
	f32 DistanceSquared = 0.0f;
	const vec3& Center = _Sphere.GetCenter();

	if (Center.x < GetMin().x)
	{
		t = Center.x - GetMin().x;
		DistanceSquared += t * t;
	}
	else if (Center.x > GetMax().x)
	{
		t = Center.x - GetMax().x;
		DistanceSquared += t * t;
	}

	if (Center.y < GetMin().y)
	{
		t = Center.y - GetMin().y;
		DistanceSquared += t * t;
	}
	else if (Center.y > GetMax().y)
	{
		t = Center.y - GetMax().y;
		DistanceSquared += t * t;
	}

	if (Center.z < GetMin().z)
	{
		t = Center.z - GetMin().z;
		DistanceSquared += t * t;
	}
	else if (Center.z > GetMax().z)
	{
		t = Center.z - GetMax().z;
		DistanceSquared += t * t;
	}

	f32 R = _Sphere.GetRadius();
	if (DistanceSquared >= R * R)
	{
		return IntersectionResult(IntersectionResult::OUTSIDE);
	}
	else if (Center.x - R < GetMin().x || Center.x + R > GetMax().x || 
			 Center.y - R < GetMin().y || Center.y + R > GetMax().y || 
			 Center.z - R < GetMin().z || Center.z + R > GetMax().z)
	{
		return IntersectionResult(IntersectionResult::INTERSECTS);
	}
	return IntersectionResult(IntersectionResult::INSIDE);
}

IntersectionResult BoundingBox::IsInsideFast( const Sphere& _Sphere ) const
{
	f32 t;
	f32 DistanceSquared = 0.0f;
	const vec3& Center = _Sphere.GetCenter();

	if (Center.x < GetMin().x)
	{
		t = Center.x - GetMin().x;
		DistanceSquared += t * t;
	}
	else if (Center.x > GetMax().x)
	{
		t = Center.x - GetMax().x;
		DistanceSquared += t * t;
	}
	if (Center.y < GetMin().y)
	{
		t = Center.y - GetMin().y;
		DistanceSquared += t * t;
	}
	else if (Center.y > GetMax().y)
	{
		t = Center.y - GetMax().y;
		DistanceSquared += t * t;
	}
	if (Center.z < GetMin().z)
	{
		t = Center.z - GetMin().z;
		DistanceSquared += t * t;
	}
	else if (Center.z > GetMax().z)
	{
		t = Center.z - GetMax().z;
		DistanceSquared += t * t;
	}

	f32 R = _Sphere.GetRadius();
	if (DistanceSquared >= R * R)
		return IntersectionResult(IntersectionResult::OUTSIDE);
	else
		return IntersectionResult(IntersectionResult::INSIDE);
}

}
